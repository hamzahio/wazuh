/* Copyright (C) 2015-2021, Wazuh Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#ifndef AGENTD_STATE_H 
#define AGENTD_STATE_H

/* Time output */
#define W_AGENTD_STATE_TIME_FORMAT "%Y-%m-%d %H:%M:%S" ///< Time format for the JSON and the file output
#define W_AGENTD_STATE_TIME_LENGHT (19 + 1)            ///< Maximum time size

/* The fields of the responses and store */
#define W_AGENTD_JSON_ERROR       "error"          ///< An error code
#define W_AGENTD_JSON_DATA        "data"           ///< The information of the response
#define W_AGENTD_FIELD_STATUS     "status"         ///< Agent status
#define W_AGENTD_FIELD_KEEP_ALIVE "last_keepalive" ///< Last time a keepalive was sent
#define W_AGENTD_FIELD_LAST_ACK   "last_ack"       ///< Last time a control message was received
#define W_AGENTD_FIELD_MSG_COUNT  "msg_count"      ///< Number of generated events
#define W_AGENTD_FIELD_MSG_SENT   "msg_sent"       ///< Number of messages sent to the manager

#include "shared.h"
#include "read-agents.h"
#include "agentd.h"

/**
 * @brief Represent the update field of the statistics
 */
typedef enum {
    UPDATE_STATUS = 0,
    UPDATE_KEEPALIVE,
    UPDATE_ACK,
    INCREMENT_MSG_COUNT,
    INCREMENT_MSG_SEND
} w_agentd_state_update_t;

/**
 * @brief A agent_state_t instance stores agent statistics
 */
typedef struct agent_state_t {
    agent_status_t status;  ///< Agent status
    time_t last_keepalive;  ///< Last time a keepalive was sent
    time_t last_ack;        ///< Last time a control message was received
    unsigned int msg_count; ///< Number of generated events
    unsigned int msg_sent;  ///< Number of messages (events + control messages) sent to the manager
} agent_state_t;

/**
 * @brief Configure and initialize statistics
 */
void w_agentd_state_init();

/**
 * @brief Main thread, write the statistics in the file
 */
void * state_main(__attribute__((unused)) void * args);

/**
 * @brief Update agent statistics
 * @param type Action
 * @param data New data value (if required)
 */
void w_agentd_state_update(w_agentd_state_update_t type, void * data);

/**
 * @brief Returns statistics in real time
 * @return Statistics in raw json format 
 */
char * w_agentd_state_get();

#endif /* AGENTD_STATE_H */
