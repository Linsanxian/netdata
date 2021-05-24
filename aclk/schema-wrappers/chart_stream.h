// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ACLK_SCHEMA_WRAPPER_CHART_STREAM_H
#define ACLK_SCHEMA_WRAPPER_CHART_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "database/rrd.h"

typedef struct {
    char* claim_id;
    char* node_id;

    uint64_t seq_id;
    uint64_t batch_id;

    struct timeval seq_id_created_at;
} stream_charts_and_dims_t;

stream_charts_and_dims_t parse_stream_charts_and_dims(const char *data, size_t len);

typedef struct {
    char* claim_id;
    char* node_id;

    uint64_t last_seq_id;
} chart_and_dim_ack_t;

chart_and_dim_ack_t parse_chart_and_dimensions_ack(const char *data, size_t len);

enum chart_reset_reason {
    DB_EMPTY,
    SEQ_ID_NOT_EXISTS,
    TIMESTAMP_MISMATCH
};

typedef struct {
    char *claim_id;
    char *node_id;

    enum chart_reset_reason reason;
} chart_reset_t;

char *generate_reset_chart_messages(size_t *len, const chart_reset_t reset);

struct aclk_message_position {
    uint64_t sequence_id;
    struct timeval seq_id_creation_time;
    uint64_t previous_sequence_id;
};

struct chart_instance_updated {
    const char *id;
    const char *claim_id;
    const char *node_id;
    const char *name;

    struct label *label_head;

    RRD_MEMORY_MODE memory_mode;

    uint32_t update_every;
    const char * config_hash;

    struct aclk_message_position position;
};

void chart_instance_updated_destroy(struct chart_instance_updated *instance);

struct chart_dimension_updated {
    const char *id;
    const char *chart_id;
    const char *node_id;
    const char *claim_id;
    const char *name;
    struct timeval created_at;
    struct timeval last_timestamp;
    struct aclk_message_position position;
};

typedef struct {
    struct chart_instance_updated *charts;
    uint16_t chart_count;

    struct chart_dimension_updated *dims;
    uint16_t dim_count;

    uint64_t batch_id;
} charts_and_dims_updated_t;

char *generate_charts_and_dimensions_updated(size_t *len, const charts_and_dims_updated_t *updates);
char *generate_charts_updated(size_t *len, char **payloads, size_t *payload_sizes, struct aclk_message_position *new_positions);
char *generate_chart_instance_updated(size_t *len, const struct chart_instance_updated *update);
char *generate_chart_dimensions_updated(size_t *len, char **payloads, size_t *payload_sizes, struct aclk_message_position *new_positions);
char *generate_chart_dimension_updated(size_t *len, const struct chart_dimension_updated *dim);

#ifdef __cplusplus
}
#endif

#endif /* ACLK_SCHEMA_WRAPPER_CHART_STREAM_H */
