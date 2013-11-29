#ifndef _X_CONNECTION
#define _X_CONNECTION

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <xcb/xcb.h>
#include <xcb/render.h>
#include <xcb/composite.h>
#include <xcb/xcb_ewmh.h>

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

#include "data_types.hpp"
#include "x_event_handler_t.hpp"
#include "x_event_source_t.hpp"

class x_ewmh;
class x_event_source;

class x_connection : public x_event_handler_t
                   , public x_event_source_t {
  public:
    typedef std::map<xcb_mod_mask_t, std::vector<xcb_keycode_t>> modifier_map;

    x_connection(void);
    ~x_connection(void);

    xcb_connection_t * const operator()(void) const;

    Display * const dpy(void) const;
    xcb_ewmh_connection_t * ewmh(void) const;
    void select_input(xcb_window_t window, uint32_t event_mask) const;
    void update_input(xcb_window_t window, uint32_t event_mask) const;
    xcb_visualtype_t * default_visual_of_screen(void);
    xcb_visualtype_t * const find_visual(unsigned int depth);
    void flush(void) const;
    int screen_number(void) const;
    xcb_screen_t * const default_screen(void) const;
    xcb_window_t const & root_window(void) const;
    uint8_t damage_event_id(void) const;
    void grab_key(uint16_t modifiers, xcb_keysym_t keysym) const;
    void ungrab_key(uint16_t modifiers, xcb_keysym_t keysym) const;
    void grab_keyboard(void) const;
    void ungrab_keyboard(void) const;
    void grab_pointer(xcb_window_t, uint16_t) const;
    void ungrab_pointer(void) const;
    modifier_map modifier_mapping(void) const;
    xcb_keysym_t keycode_to_keysym(xcb_keycode_t keycode) const;
    xcb_keycode_t keysym_to_keycode(xcb_keysym_t keysym) const;
    std::string keysym_to_string(xcb_keysym_t keysym) const;
    std::tuple<xcb_window_t, std::vector<xcb_window_t>> query_tree(xcb_window_t parent);
    std::vector<xcb_window_t> net_client_list_stacking(void) const;
    xcb_atom_t intern_atom(const std::string & name);
    xcb_window_t net_active_window(void) const;
    void request_change_current_desktop(unsigned int desktop_id);
    void request_change_active_window(xcb_window_t window);
    // root, window; XCB_NONE ^= use root_window
    std::pair<position, position> query_pointer(const xcb_window_t & window = XCB_NONE) const;
    // XCB_NONE ^= use root_window
    rectangle get_geometry(const xcb_window_t & window = XCB_NONE) const;
    rectangle current_screen(const position & p) const;
    bool handle(xcb_generic_event_t * ge);

    // ugly :(
    void set(x_ewmh * const ewmh)
    {
      if (_ewmh) return;
      _ewmh = ewmh;
    }

    // ugly :(
    void set(x_event_source_t * const event_source)
    {
      if (_event_source) return;
      _event_source = event_source;
      _event_source->attach(0, XCB_CONFIGURE_NOTIFY, this);
    }

    void attach(priority_t p, event_id_t i, x_event_handler_t * eh);
    void detach(event_id_t, x_event_handler_t * eh);
    void run_event_loop(void);
    void shutdown(void);

  private:
    uint8_t _damage_event_id;
    int _screen_number = 0;
    xcb_window_t _root_window = 0;
    Display * _dpy = NULL;
    xcb_connection_t * _c = NULL;
    xcb_screen_t * _default_screen = NULL;

    std::vector<rectangle> _screens;
    std::unordered_map<std::string, xcb_atom_t> _atoms;

    x_ewmh * _ewmh = NULL;
    x_event_source_t * _event_source = NULL;

    void find_default_screen(void);
    void init_composite(void);
    void init_damage(void);
    void init_render(void);
    void init_xfixes(void);
    void init_xinerama(void);
    void update_xinerama(void);
};

xcb_render_pictformat_t
render_find_visual_format(const x_connection & c, xcb_visualid_t visual);

xcb_render_picture_t
make_picture(const x_connection & c, xcb_window_t window);

#endif