/*
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "dnf-command-moduleenable.h"
#include "dnf-utils.h"

#include <libsmartcols.h>
#include <unistd.h>

struct _DnfCommandModuleEnable
{
  PeasExtensionBase parent_instance;
};

static void dnf_command_moduleenable_iface_init (DnfCommandInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (DnfCommandModuleEnable,
                                dnf_command_moduleenable,
                                PEAS_TYPE_EXTENSION_BASE,
                                0,
                                G_IMPLEMENT_INTERFACE (DNF_TYPE_COMMAND,
                                                       dnf_command_moduleenable_iface_init))

static void
dnf_command_moduleenable_init (DnfCommandModuleEnable *self)
{
}

static gboolean
dnf_command_moduleenable_run (DnfCommand      *cmd,
                          int              argc,
                          char            *argv[],
                          GOptionContext  *opt_ctx,
                          DnfContext      *ctx,
                          GError         **error)
{
g_auto(GStrv) pkgs = NULL;
  const GOptionEntry opts[] = {
    { G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_STRING_ARRAY, &pkgs, NULL, NULL },
    { NULL }
  };
  g_option_context_add_main_entries (opt_ctx, opts, NULL);

  if (!g_option_context_parse (opt_ctx, &argc, &argv, error))
    return FALSE;

  if (pkgs == NULL)
    {
      g_set_error_literal (error,
                           G_IO_ERROR,
                           G_IO_ERROR_FAILED,
                           "Packages are not specified");
      return FALSE;
    }
  if (!dnf_context_enable_modules(ctx, pkgs, error)) {
    return TRUE;
  }
  if (!dnf_utils_print_transaction (ctx))
    return TRUE;
  if (!dnf_context_run (ctx, NULL, error))
    return FALSE;
  return TRUE;
}

static void
dnf_command_moduleenable_class_init (DnfCommandModuleEnableClass *klass)
{
}

static void
dnf_command_moduleenable_iface_init (DnfCommandInterface *iface)
{
  iface->run = dnf_command_moduleenable_run;
}

static void
dnf_command_moduleenable_class_finalize (DnfCommandModuleEnableClass *klass)
{
}

G_MODULE_EXPORT void
dnf_command_moduleenable_register_types (PeasObjectModule *module)
{
  dnf_command_moduleenable_register_type (G_TYPE_MODULE (module));

  peas_object_module_register_extension_type (module,
                                              DNF_TYPE_COMMAND,
                                              DNF_TYPE_COMMAND_MODULEENABLE);
}
