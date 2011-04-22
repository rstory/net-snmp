#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "deliverByNotify.h"

void parse_deliver_config(const char *, char *);
void free_deliver_config(void);

deliver_by_notify test_notify;
oid test_oid[] = {1, 3, 6, 1, 6, 3, 1, 2, 2, 6}; 

/** Initializes the mteTrigger module */
void
init_deliverByNotify(void)
{
    snmpd_register_config_handler("deliver",
                                  &parse_deliver_config, &free_deliver_config,
                                  "foo");

    test_notify.frequency = 15;
    test_notify.last_run = time(NULL);
    test_notify.target = malloc(sizeof(test_oid));
    memcpy(test_notify.target, test_oid, sizeof(test_oid));
    test_notify.target_size = OID_LENGTH(test_oid);
    test_notify.max_packet_size = -1;

    snmp_alarm_register(calculate_time_until_next_run(&test_notify, NULL), 0, 
                        &deliver_execute, NULL);
}

void
parse_deliver_config(const char *token, char *line) {
}

void
free_deliver_config(void) {
}

void
deliver_execute(unsigned int clientreg, void *clientarg) {
    snmp_log(LOG_ERR, "got here: deliver by notify\n");
}

int
calculate_time_until_next_run(deliver_by_notify *it, time_t *now) {
    time_t          local_now;
    int             time_since_last;

    /* if we weren't passed a valid time, fake it */
    if (NULL == now) {
        now = &local_now;
        time(&local_now);
    }

    time_since_last = local_now - it->last_run;

    return it->frequency - time_since_last;
}

