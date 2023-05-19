/* mail-endpoint.c
 *
 * Copyright 2023 Vitaly Dyachkov <obyknovenius@me.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "mail-endpoint.h"

struct _MailEndpoint
{
  GObject parnet_instance;

  GSocketClient *socket_client;
};

G_DEFINE_FINAL_TYPE (MailEndpoint, mail_endpoint, G_TYPE_OBJECT)

MailEndpoint *
mail_endpoint_new (void)
{
  return g_object_new (MAIL_TYPE_ENDPOINT,
                       NULL);
}

static GSocketClient *
get_socket_client (MailEndpoint *self)
{
  if (self->socket_client)
    return self->socket_client;

  self->socket_client = g_socket_client_new ();

  return self->socket_client;
}

void
mail_endpoint_connect_async (MailEndpoint       *self,
                             GCancellable       *cancellable,
                             GAsyncReadyCallback callback,
                             gpointer            user_data)
{
  GSocketClient *socket_client;

  socket_client = get_socket_client (self);

  g_socket_client_connect_to_host_async (socket_client,
                                         "mail.igalia.com", 993,
                                         cancellable, callback, user_data);
}

GSocketConnection *
mail_endpoint_connect_finish (MailEndpoint *self,
                              GAsyncResult *result,
                              GError      **error)
{
  return g_socket_client_connect_to_host_finish (self->socket_client,
                                                 result,
                                                 error);
}

static void
mail_endpoint_class_init (MailEndpointClass *klass)
{
}

static void
mail_endpoint_init (MailEndpoint *self)
{
}
