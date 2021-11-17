#include "mathlib.h"
#include <gtkmm.h>

#include <string>
#include <vector>
#include <iostream>


namespace gtk = Gtk;
namespace glib = Glib;

class WindowMain : public gtk::Window {
public:
  WindowMain() {
    add(scrolledWindow);
    scrolledWindow.add(fixed);
    
    int start_y = 20;
    int h_y = 35;

    entry_vec = std::vector<gtk::Entry>(num_of_entry);
    coef_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    const auto la = [&](){
      if(last_entry+1 < num_of_entry)
      {
        entry_vec[++last_entry].show();
        coef_entry_vec[last_entry].show();
      }
    };

    for(int i = 0; i < num_of_entry; ++i){
      entry_vec[i].signal_activate().connect(la);
      entry_vec[i].set_size_request(250,30);
      fixed.add(coef_entry_vec[i]);
      fixed.add(entry_vec[i]);
      fixed.move(entry_vec[i], 20, start_y + i * h_y);
      fixed.move(coef_entry_vec[i], 300, start_y + i * h_y);
    }

    
    range = std::vector<gtk::Entry>(2);
    fixed.add(range[0]);
    fixed.add(range[1]);
    fixed.move(range[0], 600, 20);
    fixed.move(range[1], 600, 50);

    button1.set_label("Calculate");
    button1.signal_button_release_event().connect([&](GdkEventButton*) {
      std::string str_arg = "x";


      std::vector<std::string> func_vec;
      std::vector<double> coef_vec;
      std::vector<std::pair<double,double>> func_range;

      if(range[0].get_text() == "" || range[1].get_text() == "") return true;
      
      double x0 = std::stod(range[0].get_text()), x1 = std::stod(range[1].get_text());
      func_range.push_back(std::make_pair(x0,x1));

      for(int i = 0; i < entry_vec.size(); ++i){
        std::string tmp_string(entry_vec[i].get_text());
        std::string coef(coef_entry_vec[i].get_text());

        if(tmp_string != "" && coef != ""){
          func_vec.push_back(tmp_string);
          coef_vec.push_back(std::stod(coef));
        }
      }
      if(func_vec.size() == 0) return true;

      FunctionContainer MO_method(func_vec, str_arg, func_range);
      auto res = MO_method.Convolution(coef_vec, 1e-3);

      num_of_iter.set_label("Number of iteration: " + std::to_string(res.num_iteration));
      point.set_label("Min at x: " + std::to_string(res.return_point[0]));
      value.set_label("Function value: " + std::to_string(res.value));

      MO_method.DrawPlot(coef_vec, 1e-3, true ,"Conv");
      return true;
      });

    point.set_label("Min at x: ");
    fixed.add(point);
    fixed.move(point, 600, 380);

    value.set_label("Function value: ");
    fixed.add(value);
    fixed.move(value, 600, 415);

    num_of_iter.set_label("Number of iteration: ");
    fixed.add(num_of_iter);
    fixed.move(num_of_iter, 600, 450);

    fixed.add(button1);
    fixed.move(button1, 20, 450);
    
    set_title("Program");
    resize(800, 500);
    show_all();
    for(auto& c : entry_vec)
      c.hide();
    for(auto& c : coef_entry_vec)
      c.hide();
    entry_vec[0].show();
    coef_entry_vec[0].show();
  }
  
private:
  gtk::Fixed fixed;
  gtk::ScrolledWindow scrolledWindow;
  gtk::Button button1;
  std::vector<gtk::Entry> entry_vec;
  std::vector<gtk::Entry> coef_entry_vec;
  std::vector<gtk::Entry> range;
  gtk::Label num_of_iter;
  gtk::Label point;
  gtk::Label value;
  int num_of_entry = 10;
  int last_entry = 0;
  int button1Clicked = 0;
  int button2Clicked = 0;
  
};

int main(int argc, char* argv[]) {
  g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", true, NULL);
  g_object_set(gtk_settings_get_default(), "gtk-button-images", true, NULL);
  auto application = gtk::Application::create(argc, argv);
  WindowMain window;
  return application->run(window);
}