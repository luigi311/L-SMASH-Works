/*****************************************************************************
 * lwlibav_video.h
 *****************************************************************************
 * Copyright (C) 2012-2013 L-SMASH Works project
 *
 * Authors: Yusuke Nakamura <muken.the.vfrmaniac@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/

/* This file is available under an ISC license. */

typedef struct
{
    uint8_t  keyframe;
    uint8_t  is_leading;
    uint32_t sample_number;     /* unique value in decoding order */
    int64_t  pts;
    int64_t  dts;
    int64_t  file_offset;
} video_frame_info_t;

typedef struct
{
    uint32_t decoding_to_presentation;
} order_converter_t;

typedef struct
{
    AVFormatContext    *format;
    int                 stream_index;
    uint32_t            forward_seek_threshold;
    int                 seek_mode;
    /* */
    error_handler_t     eh;
    AVCodecContext     *ctx;
    AVIndexEntry       *index_entries;
    int                 index_entries_count;
    int                 seek_flags;
    int                 seek_base;
    int                 max_width;
    int                 max_height;
    int                 initial_width;
    int                 initial_height;
    enum AVPixelFormat  initial_pix_fmt;
    enum AVColorSpace   initial_colorspace;
    enum AVCodecID      codec_id;
    uint32_t            frame_count;
    uint32_t            delay_count;
    AVPacket            packet;
    AVFrame            *frame_buffer;
    order_converter_t  *order_converter;    /* stored in decoding order */
    uint8_t            *keyframe_list;      /* stored in decoding order */
    video_frame_info_t *frame_list;         /* stored in presentation order */
    uint32_t            last_frame_number;
    uint32_t            last_rap_number;
} lwlibav_video_decode_handler_t;

int lwlibav_get_desired_video_track
(
    const char                     *file_path,
    lwlibav_video_decode_handler_t *vdhp,
    int                             threads
);

void lwlibav_setup_timestamp_info
(
    lwlibav_video_decode_handler_t *vdhp,
    int                            *framerate_num,
    int                            *framerate_den
);

void lwlibav_find_random_accessible_point
(
    lwlibav_video_decode_handler_t *vdhp,
    uint32_t                        presentation_sample_number,
    uint32_t                        decoding_sample_number,
    uint32_t                       *rap_number
);

int64_t lwlibav_get_random_accessible_point_position
(
    lwlibav_video_decode_handler_t *vdhp,
    uint32_t                        rap_number
);

int lwlibav_get_video_frame
(
    lwlibav_video_decode_handler_t *vdhp,
    uint32_t                        frame_number,
    uint32_t                        frame_count
);

void lwlibav_cleanup_video_decode_handler( lwlibav_video_decode_handler_t *vdhp );