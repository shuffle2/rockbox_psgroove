/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Tuner "middleware" for RDA5802 chip present in some Sansa Clip+ players
 *
 * Copyright (C) 2010 Bertrik Sikken
 * Copyright (C) 2008 Nils Wallménius (si4700 code that this was based on)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "config.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "kernel.h"
#include "tuner.h" /* tuner abstraction interface */
#include "fmradio.h"
#include "fmradio_i2c.h" /* physical interface driver */

#define SEEK_THRESHOLD 0x16

#define I2C_ADR 0x20

/** Registers and bits **/
#define POWERCFG    0x2
#define CHANNEL     0x3
#define SYSCONFIG1  0x4
#define SYSCONFIG2  0x5
#define SYSCONFIG3  0x6

#define READCHAN    0xA
#define STATUSRSSI  0xB
#define IDENT       0xC


/* POWERCFG (0x2) */
#define POWERCFG_DMUTE      (0x1 << 14)
#define POWERCFG_MONO       (0x1 << 13)
#define POWERCFG_SOFT_RESET (0x1 <<  1)
#define POWERCFG_ENABLE     (0x1 <<  0)

/* CHANNEL (0x3) */
#define CHANNEL_CHAN        (0x3ff << 6)
    #define CHANNEL_CHANw(x) (((x) << 6) & CHANNEL_CHAN)
#define CHANNEL_TUNE        (0x1 << 4)
#define CHANNEL_BAND        (0x3 << 2)
    #define CHANNEL_BANDw(x)        (((x) << 2) & CHANNEL_BAND)
    #define CHANNEL_BANDr(x)        (((x) & CHANNEL_BAND) >> 2)
    #define CHANNEL_BAND_870_1080   (0x0) /* tenth-megahertz */
    #define CHANNEL_BAND_760_1080   (0x1)
    #define CHANNEL_BAND_760_900    (0x2)
    #define CHANNEL_BAND_650_760    (0x3)
#define CHANNEL_SPACE       (0x3 << 0)
    #define CHANNEL_SPACEw(x)  (((x) << 0) & CHANNEL_SPACE)
    #define CHANNEL_SPACEr(x)  (((x) & CHANNEL_SPACE) >> 0)
    #define CHANNEL_SPACE_100KHZ    (0x0)
    #define CHANNEL_SPACE_200KHZ    (0x1)
    #define CHANNEL_SPACE_50KHZ     (0x2)

/* SYSCONFIG1 (0x4) */
#define SYSCONFIG1_DE       (0x1 << 11)
    #define SYSCONFIG1_SMUTE (0x1 << 9)

/* SYSCONFIG2 (0x5) */
#define SYSCONFIG2_VOLUME   (0xF <<  0)

/* READCHAN (0xA) */
#define READCHAN_READCHAN   (0x3ff << 0)
    #define READCHAN_READCHANr(x) (((x) & READCHAN_READCHAN) >> 0)
#define READCHAN_STC        (0x1 << 14)
#define READCHAN_ST         (0x1 << 10)

/* STATUSRSSI (0xB) */
#define STATUSRSSI_RSSI     (0x7F << 9)
    #define STATUSRSSI_RSSIr(x) (((x) & STATUSRSSI_RSSI) >> 9)
#define STATUSRSSI_FM_TRUE  (0x1 << 8)

static const uint16_t initvals[16] = {
    0x0000, 0x0000, 0xC401, 0x1B90, 
    0x0400, 0x866F, 0x8000, 0x4712, 
    0x5EC6, 0x0000, 0x406E, 0x2D80, 
    0x5803, 0x5804, 0x5804, 0x5804
};

static bool tuner_present = false;
static int curr_frequency = 87500000; /* Current station frequency (HZ) */
static uint16_t cache[16];

/* reads <len> registers from radio at offset 0x0A into cache */
static void rda5802_read(int len)
{
    int i;
    unsigned char buf[128];
    unsigned char *ptr = buf;
    uint16_t data;
    
    fmradio_i2c_read(I2C_ADR, buf, len * 2);
    for (i = 0; i < len; i++) {
        data = ptr[0] << 8 | ptr[1];
        cache[READCHAN + i] = data;
        ptr += 2;
    }
}

/* writes <len> registers from cache to radio at offset 0x02 */
static void rda5802_write(int len)
{
    int i;
    unsigned char buf[64];
    unsigned char *ptr = buf;
    uint16_t data;

    for (i = 0; i < len; i++) {
        data = cache[POWERCFG + i];
        *ptr++ = (data >> 8) & 0xFF;
        *ptr++ = data & 0xFF;
    }
    fmradio_i2c_write(I2C_ADR, buf, len * 2);
}

static uint16_t rda5802_read_reg(int reg)
{
    rda5802_read((reg - READCHAN) + 1);
    return cache[reg];
}

static void rda5802_write_reg(int reg, uint16_t value)
{
    cache[reg] = value;
}

static void rda5802_write_cache(void)
{
    rda5802_write(5);
}

static void rda5802_write_masked(int reg, uint16_t bits, uint16_t mask)
{
    rda5802_write_reg(reg, (cache[reg] & ~mask) | (bits & mask));
}

static void rda5802_write_clear(int reg, uint16_t mask)
{
    rda5802_write_reg(reg, cache[reg] & ~mask);
}

static void rda5802_write_set(int reg, uint16_t mask)
{
    rda5802_write_reg(reg, cache[reg] | mask);
}

static void rda5802_sleep(int snooze)
{
    if (snooze) {
        rda5802_write_clear(POWERCFG, POWERCFG_ENABLE);
    }
    else {
        rda5802_write_set(POWERCFG, POWERCFG_ENABLE);
    }
    rda5802_write_cache();
}

bool rda5802_detect(void)
{
    return ((rda5802_read_reg(IDENT) & 0xFF00) == 0x5800);
 }

void rda5802_init(void)
{
    if (rda5802_detect()) {
        tuner_present = true;

        // soft-reset
        rda5802_write_reg(POWERCFG, POWERCFG_SOFT_RESET);
        rda5802_write(1);
        sleep(HZ * 10 / 1000);

        // write initialisation values
        memcpy(cache, initvals, sizeof(cache));
        rda5802_write(14);
        sleep(HZ * 70 / 1000);
    }
}

static void rda5802_set_frequency(int freq)
{
    int i;
    uint16_t readchan;

    /* check BAND and spacings */
    int start = CHANNEL_BANDr(cache[CHANNEL]) & 1 ? 76000000 : 87000000;
    int chan = (freq - start) / 50000;

    curr_frequency = freq;

    for (i = 0; i < 5; i++) {
        /* tune and wait a bit */
        rda5802_write_masked(CHANNEL, CHANNEL_CHANw(chan) | CHANNEL_TUNE,
                                CHANNEL_CHAN | CHANNEL_TUNE);
        rda5802_write_cache();
        sleep(HZ * 70 / 1000);
        rda5802_write_clear(CHANNEL, CHANNEL_TUNE);
        rda5802_write_cache();
        
        /* check if tuning was successful */
        readchan = rda5802_read_reg(READCHAN);
        if (readchan & READCHAN_STC) {
            if (READCHAN_READCHANr(readchan) == chan) {
                break;
            }
        }
    }
}

static int rda5802_tuned(void)
{
    /* Primitive tuning check: sufficient level and AFC not railed */
    uint16_t status = rda5802_read_reg(STATUSRSSI);
    if (STATUSRSSI_RSSIr(status) >= SEEK_THRESHOLD &&
        (status & STATUSRSSI_FM_TRUE)) {
        return 1;
    }

    return 0;
}

static void rda5802_set_region(int region)
{
    const struct fm_region_data *rd = &fm_region_data[region];
    int band = (rd->freq_min == 76000000) ?
               CHANNEL_BAND_760_900 : CHANNEL_BAND_870_1080;
    int deemphasis = (rd->deemphasis == 50) ? SYSCONFIG1_DE : 0;

    uint16_t bandspacing = CHANNEL_BANDw(band) |
                           CHANNEL_SPACEw(CHANNEL_SPACE_50KHZ);
    uint16_t oldbs = cache[CHANNEL] & (CHANNEL_BAND | CHANNEL_SPACE);

    rda5802_write_masked(SYSCONFIG1, deemphasis, SYSCONFIG1_DE);
    rda5802_write_masked(CHANNEL, bandspacing, CHANNEL_BAND | CHANNEL_SPACE);
    rda5802_write_cache();

    /* Retune if this region change would change the channel number. */
    if (oldbs != bandspacing) {
        rda5802_set_frequency(curr_frequency);
    }
}

static bool rda5802_st(void)
{
    return (rda5802_read_reg(READCHAN) & READCHAN_ST);
}

/* tuner abstraction layer: set something to the tuner */
int rda5802_set(int setting, int value)
{
    switch (setting) {
    case RADIO_SLEEP:
        if (value != 2) {
            rda5802_sleep(value);
        }
        break;

    case RADIO_FREQUENCY:
        rda5802_set_frequency(value);
        break;

    case RADIO_SCAN_FREQUENCY:
        rda5802_set_frequency(value);
        return rda5802_tuned();

    case RADIO_MUTE:
        rda5802_write_masked(POWERCFG, value ? 0 : POWERCFG_DMUTE,
                                POWERCFG_DMUTE);
        rda5802_write_masked(SYSCONFIG1, (3 << 9), (3 << 9));
        rda5802_write_set(SYSCONFIG2, SYSCONFIG2_VOLUME);
        rda5802_write_cache();
        break;

    case RADIO_REGION:
        rda5802_set_region(value);
        break;

    case RADIO_FORCE_MONO:
        rda5802_write_masked(POWERCFG, value ? POWERCFG_MONO : 0,
                            POWERCFG_MONO);
        rda5802_write_cache();
        break;

    default:
        return -1;
    }

    return 1;
}

/* tuner abstraction layer: read something from the tuner */
int rda5802_get(int setting)
{
    int val = -1; /* default for unsupported query */

    switch (setting) {
    case RADIO_PRESENT:
        val = tuner_present ? 1 : 0;
        break;

    case RADIO_TUNED:
        val = rda5802_tuned();
        break;

    case RADIO_STEREO:
        val = rda5802_st();
        break;
    }

    return val;
}

void rda5802_dbg_info(struct rda5802_dbg_info *nfo)
{
    rda5802_read(6);
    memcpy(nfo->regs, cache, sizeof (nfo->regs));
}

