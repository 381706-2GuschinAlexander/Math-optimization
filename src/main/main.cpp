#include "mathlib.h"
#include <gtkmm.h>

#include <string>
#include <vector>

namespace gtk = Gtk;
namespace glib = Glib;

class WindowMain : public gtk::Window {
public:
  WindowMain() {
    add(scrolledWindow);
    scrolledWindow.add(fixed);
    
    button1.set_label("button1");
    button1.signal_button_release_event().connect([&](GdkEventButton*) {
      std::string str_func = "x*sin(x)+10";
      std::string str_arg = "x";
      std::vector<std::string> f(1, str_func);
      FunctionContainer A(f, str_arg);
      A.Convolution();
      return true;
    });
    fixed.add(button1);
    fixed.move(button1, 50, 50);
    

    label1.set_text(glib::ustring::compose("button1 clicked %1 times", button1Clicked));
    fixed.add(label1);
    fixed.move(label1, 50, 200);
    
    
    set_title("Programm");
    resize(300, 300);
    show_all();
  }
  
private:
  gtk::Fixed fixed;
  gtk::ScrolledWindow scrolledWindow;
  gtk::Button button1;
  gtk::Label label1;
  int button1Clicked = 0;
  int button2Clicked = 0;
  
};

int main(int argc, char* argv[]) {
    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", true, NULL);
    g_object_set(gtk_settings_get_default(), "gtk-button-images", true, NULL);
  //gtk_settings_set_long_property(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", 0, nullptr);
  auto application = gtk::Application::create(argc, argv);
  WindowMain window;
  return application->run(window);
}