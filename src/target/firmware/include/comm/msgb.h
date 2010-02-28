#ifndef _MSGB_H
#define _MSGB_H

/* (C) 2008-2010 by Harald Welte <laforge@gnumonks.org>
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <linuxlist.h>

struct msgb {
	struct llist_head list;

	/* the A-bis layer 2 header: OML, RSL(RLL), NS */
	unsigned char *l2h;
	/* the layer 3 header. For OML: FOM; RSL: 04.08; GPRS: BSSGP */
	unsigned char *l3h;

	uint16_t data_len;
	uint16_t len;

	unsigned char *head;	/* start of buffer */
	unsigned char *tail;	/* end of message */
	unsigned char *data;	/* start of message */
	unsigned char _data[0];
};

extern struct msgb *msgb_alloc(uint16_t size, const char *name);
extern void msgb_free(struct msgb *m);
extern void msgb_enqueue(struct llist_head *queue, struct msgb *msg);
extern struct msgb *msgb_dequeue(struct llist_head *queue);
extern void msgb_reset(struct msgb *m);

#define msgb_l1(m)	((void *)(m->l1h))
#define msgb_l2(m)	((void *)(m->l2h))
#define msgb_l3(m)	((void *)(m->l3h))

static inline unsigned int msgb_l1len(const struct msgb *msgb)
{
	return msgb->tail - (uint8_t *)msgb_l1(msgb);
}

static inline unsigned int msgb_l2len(const struct msgb *msgb)
{
	return msgb->tail - (uint8_t *)msgb_l2(msgb);
}

static inline unsigned int msgb_l3len(const struct msgb *msgb)
{
	return msgb->tail - (uint8_t *)msgb_l3(msgb);
}

static inline unsigned int msgb_headlen(const struct msgb *msgb)
{
	return msgb->len - msgb->data_len;
}
static inline int msgb_tailroom(const struct msgb *msgb)
{
	return (msgb->head + msgb->data_len) - msgb->tail;
}
static inline unsigned char *msgb_put(struct msgb *msgb, unsigned int len)
{
	unsigned char *tmp = msgb->tail;

	if (msgb_tailroom(msgb) < len)
		cons_puts("msgb_tailroom insufficient!\n");

	msgb->tail += len;
	msgb->len += len;
	return tmp;
}
static inline unsigned char *msgb_push(struct msgb *msgb, unsigned int len)
{
	msgb->data -= len;
	msgb->len += len;
	return msgb->data;
}
static inline unsigned char *msgb_pull(struct msgb *msgb, unsigned int len)
{
	msgb->len -= len;
	return msgb->data += len;
}

/* increase the headroom of an empty msgb, reducing the tailroom */
static inline void msgb_reserve(struct msgb *msg, int len)
{
	msg->data += len;
	msg->tail += len;
}

static inline struct msgb *msgb_alloc_headroom(int size, int headroom,
						const char *name)
{
	struct msgb *msg = msgb_alloc(size, name);
	if (msg)
		msgb_reserve(msg, headroom);
	return msg;
}

#endif /* _MSGB_H */
