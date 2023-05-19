/* mail-application.c
 *
 * Copyright 2022 Vitaly
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

#include "mail-application.h"
#include "mail-window.h"
#include "mail-endpoint.h"

struct _MailApplication
{
  AdwApplication parent_instance;
};

G_DEFINE_TYPE (MailApplication, mail_application, ADW_TYPE_APPLICATION)

MailApplication *
mail_application_new (const char        *application_id,
                      GApplicationFlags  flags)
{
  g_return_val_if_fail (application_id != NULL, NULL);

  return g_object_new (MAIL_TYPE_APPLICATION,
                       "application-id", application_id,
                       "flags", flags,
                       NULL);
}

static void
mail_application_startup (GApplication *app)
{
  GtkCssProvider *css_provider;

  g_assert (MAIL_IS_APPLICATION (app));

  G_APPLICATION_CLASS (mail_application_parent_class)->startup (app);

  css_provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_resource (css_provider, "/com/obyknovenius/Mail/style.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

static void
endpoint_connect_finish_cb (GObject      *source_object,
                            GAsyncResult *result,
                            gpointer      user_data)
{
  MailEndpoint *endpoint;
  GSocketConnection *connection;

  endpoint = MAIL_ENDPOINT(source_object);

  connection = mail_endpoint_connect_finish (endpoint, result, user_data);
  if (connection)
    {
      g_print("Connected\n");
    }
  else
    {
      g_print("Connection failed\n");
    }
}

static void
mail_application_activate (GApplication *app)
{
  GtkWindow *window;
  MailEndpoint *endpoint;

  g_assert (MAIL_IS_APPLICATION (app));

  window = gtk_application_get_active_window (GTK_APPLICATION (app));
  if (window == NULL)
    window = g_object_new (MAIL_TYPE_WINDOW,
                           "application", app,
                           NULL);

  gtk_window_present (window);

  endpoint = mail_endpoint_new ();
  mail_endpoint_connect_async (endpoint, NULL, endpoint_connect_finish_cb, NULL);
}

static void
mail_application_class_init (MailApplicationClass *klass)
{
  GApplicationClass *app_class = G_APPLICATION_CLASS (klass);

  app_class->startup = mail_application_startup;
  app_class->activate = mail_application_activate;
}

static void
mail_application_about_action (GSimpleAction *action,
                               GVariant      *parameter,
                               gpointer       user_data)
{
  static const char *developers[] = {"Vitaly", NULL};
  MailApplication *self = user_data;
  GtkWindow *window = NULL;

  g_assert (MAIL_IS_APPLICATION (self));

  window = gtk_application_get_active_window (GTK_APPLICATION (self));

  adw_show_about_window (window,
                         "application-name", "mail",
                         "application-icon", "com.obyknovenius.Mail",
                         "developer-name", "Vitaly",
                         "version", "0.1.0",
                         "developers", developers,
                         "copyright", "© 2022 Vitaly",
                         NULL);
}

static void
mail_application_quit_action (GSimpleAction *action,
                              GVariant      *parameter,
                              gpointer       user_data)
{
  MailApplication *self = user_data;

  g_assert (MAIL_IS_APPLICATION (self));

  g_application_quit (G_APPLICATION (self));
}

static const GActionEntry app_actions[] = {
  { "quit", mail_application_quit_action },
  { "about", mail_application_about_action },
};

static void
mail_application_init (MailApplication *self)
{
  g_action_map_add_action_entries (G_ACTION_MAP (self),
                                   app_actions,
                                   G_N_ELEMENTS (app_actions),
                                   self);
  gtk_application_set_accels_for_action (GTK_APPLICATION (self),
                                         "app.quit",
                                         (const char *[]) { "<primary>q", NULL });
}

