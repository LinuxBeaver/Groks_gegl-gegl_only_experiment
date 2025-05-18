/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *

THIS PLUGIN IS NOT FOR NORMAL USE, IT IS PROOF BY GROK THAT WE CAN MAKE A GEGL PLUGINS IN A RADICALLY DIFFERENT WAY 
 BY CALLING GEGL:GEGL AND MODIFYING ITS CONTENT

All this plugin does is call color overlay then dropshadow

its name is smooth.c because i'm lazy
 */

#include "config.h"
#include <glib/gi18n-lib.h>
#include <stdio.h>

#ifdef GEGL_PROPERTIES

property_color (color, _("Overlay Color"), "red")
  description (_("Color for the overlay"))

property_double (grow_radius, _("Shadow Grow Radius"), 10.0)
  description (_("Size of the drop shadow's grow radius"))
  value_range (1.0, 20.0)
  ui_range    (1.0, 20.0)

property_double (opacity, _("Shadow Opacity"), 1.0)
  description (_("Opacity of the drop shadow"))
  value_range (0.0, 1.0)
  ui_range    (0.0, 1.0)

property_double (blur_radius, _("Shadow Blur Radius"), 0.0)
  description (_("Blur radius for the drop shadow"))
  value_range (0.0, 15.0)
  ui_range    (0.0, 15.0)

property_double (x_offset, _("Shadow X Offset"), 0.0)
  description (_("Horizontal offset of the drop shadow"))
  value_range (-20.0, 20.0)
  ui_range    (-20.0, 20.0)

property_double (y_offset, _("Shadow Y Offset"), 0.0)
  description (_("Vertical offset of the drop shadow"))
  value_range (-20.0, 20.0)
  ui_range    (-20.0, 20.0)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     smooth
#define GEGL_OP_C_SOURCE smooth.c

#include "gegl-op.h"

typedef struct
{
  GeglNode *input;
  GeglNode *smooth;
  GeglNode *output;
} State;

/* Forward declaration */
static void update_graph (GeglOperation *operation);

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);

  State *state = o->user_data = g_malloc0 (sizeof (State));
  if (!state)
  {
    g_warning ("Smooth: Failed to allocate State");
    return;
  }

  state->input = gegl_node_get_input_proxy (gegl, "input");
  state->smooth = gegl_node_new_child (gegl, "operation", "gegl:gegl", NULL);
  state->output = gegl_node_get_output_proxy (gegl, "output");

  /* Initialize the graph with default property values */
  update_graph (operation);

  /* Force a property update to trigger initial rendering */
  g_object_notify (G_OBJECT (operation), "blur_radius");
}

static void update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  /* Get the color as RGBA and convert to hex */
  GeglColor *gegl_color = o->color;
  gdouble r, g, b, a;
  gegl_color_get_rgba (gegl_color, &r, &g, &b, &a);
  gchar *color_hex = g_strdup_printf ("#%02x%02x%02x",
                                      (int)(r * 255),
                                      (int)(g * 255),
                                      (int)(b * 255));

  /* Format the graph with color (color-overlay), grow_radius, blur_radius, opacity, x_offset, y_offset */
  gchar *smooth = g_strdup_printf (
    "color-overlay value=%s dropshadow x=%f y=%f radius=%f grow-radius=%f opacity=%f color=white",
    color_hex,
    o->x_offset,
    o->y_offset,
    o->blur_radius,
    o->grow_radius,
    o->opacity
  );

  /* Debug: Log the graph string */
  g_debug ("Smooth: Setting graph string: %s", smooth);

  /* Set the graph string and check for errors */
  GError *error = NULL;
  gegl_node_set (state->smooth, "string", smooth, NULL);
  if (error) {
    g_warning ("Smooth: Failed to set graph: %s", error->message);
    g_clear_error (&error);
  }

  gegl_node_link_many (state->input, state->smooth, state->output, NULL);

  /* Debug: Log property values */
  g_debug ("Smooth: color=%s, grow_radius=%f, opacity=%f, blur_radius=%f, x_offset=%f, y_offset=%f",
           color_hex, o->grow_radius, o->opacity, o->blur_radius, o->x_offset, o->y_offset);

  /* Free the formatted strings */
  g_free (color_hex);
  g_free (smooth);
}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
  GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:groktest",
    "title",       _("Grok Test"),
    "reference-hash", "smoothsmooth",
    "description", _("Apply a colored overlay with a white drop shadow"),
    NULL);
}

#endif
