#include <stdio.h>
#include <wayland-server.h>

struct wl_display *display;
struct wl_compositor *compositor;
struct wl_surface *surface;
struct wl_shell *shell;
struct wl_shell_surface *shell_surface;

void handle_surface_destroy(struct wl_listener *listener, void *data) {
  wl_list_remove(listener);
  free(listener);
}

void handle_shell_surface_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
  struct wl_listener *listener = malloc(sizeof(struct wl_listener));
  listener->notify = handle_surface_destroy;
  wl_signal_add(&surface->destroy_signal, listener);
}

void handle_new_surface(void *data, struct wl_compositor *compositor, uint32_t id) {
  surface = wl_compositor_create_surface(compositor);
  shell_surface = wl_shell_get_shell_surface(shell, surface);
  wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);
  wl_shell_surface_set_toplevel(shell_surface);
}

struct wl_listener surface_listener = {handle_new_surface};
struct wl_shell_surface_listener shell_surface_listener = {handle_shell_surface_configure};

int main() {
  display = wl_display_create();
  compositor = wl_display_get_compositor(display);
  shell = wl_display_get_shell(display);
  wl_compositor_add_listener(compositor, &surface_listener, NULL);

  wl_display_run(display);
  wl_display_destroy(display);

  return 0;
}
