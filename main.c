#include <wayland-server.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct wl_display *display;
struct wl_compositor *compositor;

struct wl_surface *focused_surface;

void surface_create(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
    struct wl_surface *surface = wl_resource_create(client, &wl_surface_interface, 1, id);

    // Set the newly created surface as the focused surface
    focused_surface = surface;
}

void surface_destroy(struct wl_client *client, struct wl_resource *resource)
{
    // Remove the reference to the destroyed surface
    if (focused_surface == (struct wl_surface *) resource)
        focused_surface = NULL;

    wl_resource_destroy(resource);
}

struct wl_surface_interface surface_impl = {
    .destroy = surface_destroy
};

struct wl_compositor_interface compositor_impl = {
    .create_surface = surface_create
};

void handle_client_connection(int fd, uint32_t mask, void *data)
{
    struct wl_client *client = wl_client_create(display, fd);
    if (!client) {
        close(fd);
        return;
    }

    wl_compositor_create(client, 1, 1);
}

int main(int argc, char **argv)
{
    display = wl_display_create();
    if (!display)
        return 1;

    compositor = wl_global_create(display, &wl_compositor_interface, 1, &compositor_impl, handle_client_connection);
    if (!compositor)
        return 1;

    if (wl_display_add_socket(display, NULL))
        return 1;

    wl_display_run(display);

    wl_display_destroy(display);
    return 0;
}
