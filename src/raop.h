
#ifndef __RAOP_H__
#define __RAOP_H__

#include <netinet/in.h>

#include <event.h>
#include "evrtsp/evrtsp.h"


union sockaddr_all
{
  struct sockaddr_in sin;
  struct sockaddr_in6 sin6;
  struct sockaddr sa;
  struct sockaddr_storage ss;
};

struct raop_session;

struct raop_device
{
  uint64_t id;
  char *name;

  char *v4_address;
  char *v6_address;
  short v4_port;
  short v6_port;

  char selected;
  char advertised;
  char encrypt;

  char has_password;
  const char *password;

  struct raop_session *session;

  struct raop_device *next;
};

/* RAOP session state */

/* Session is starting up */
#define RAOP_F_STARTUP    (1 << 15)

/* Streaming is up (connection established) */
#define RAOP_F_CONNECTED  (1 << 16)

enum raop_session_state
  {
    RAOP_STOPPED   = 0,

    /* Session startup */
    RAOP_OPTIONS   = RAOP_F_STARTUP | 0x01,
    RAOP_ANNOUNCE  = RAOP_F_STARTUP | 0x02,
    RAOP_SETUP     = RAOP_F_STARTUP | 0x03,
    RAOP_RECORD    = RAOP_F_STARTUP | 0x04,

    /* Session established
     * - streaming ready (RECORD sent and acked, connection established)
     * - commands (SET_PARAMETER) are possible
     */
    RAOP_CONNECTED = RAOP_F_CONNECTED,

    /* Audio data is being sent */
    RAOP_STREAMING = RAOP_F_CONNECTED | 0x01,

    /* Session is failed, couldn't startup or error occurred */
    RAOP_FAILED    = -1,

    /* Password issue: unknown password or bad password */
    RAOP_PASSWORD  = -2,
  };

typedef void (*raop_status_cb)(struct raop_device *dev, struct raop_session *rs, enum raop_session_state status);


int
raop_device_probe(struct raop_device *rd, raop_status_cb cb);

int
raop_device_start(struct raop_device *rd, raop_status_cb cb, uint64_t rtptime);

void
raop_device_stop(struct raop_session *rs);

void
raop_playback_start(uint64_t next_pkt, struct timespec *ts);

void
raop_playback_stop(void);


int
raop_set_volume(int volume, raop_status_cb cb);

int
raop_flush(raop_status_cb cb, uint64_t rtptime);


void
raop_set_status_cb(struct raop_session *rs, raop_status_cb cb);


void
raop_v2_write(uint8_t *buf, uint64_t rtptime);


int
raop_init(void);

void
raop_deinit(void);

#endif /* !__RAOP_H__ */
