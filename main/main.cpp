#include "mathlib.h"
#include "beam.h"
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
    
    int start_y = 35;
    int h_y = 35;

    function_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    coef_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    const auto la = [&](){
      if(last_entry+1 < num_of_entry)
      {
        function_entry_vec[++last_entry].show();
        coef_entry_vec[last_entry].show();
      }
    };

    for(int i = 0; i < num_of_entry; ++i){
      function_entry_vec[i].signal_activate().connect(la);
      function_entry_vec[i].set_size_request(250,30);
      fixed.add(coef_entry_vec[i]);
      fixed.add(function_entry_vec[i]);
      fixed.move(function_entry_vec[i], 20, start_y + i * h_y);
      fixed.move(coef_entry_vec[i], 300, start_y + i * h_y);
    }

    functions_label.set_label("f(x)");
    fixed.add(functions_label);
    fixed.move(functions_label, 125, 10);

    params_label.set_label("Weight");
    fixed.add(params_label);
    fixed.move(params_label, 355, 10);

    range_str_label.set_label("Range");
    fixed.add(range_str_label);
    fixed.move(range_str_label, 665, 10);

    pointx_label.set_label("Min at x: ");
    fixed.add(pointx_label);
    fixed.move(pointx_label, 600, 345);
    pointy_label.set_label("Min at y: ");
    fixed.add(pointy_label);
    fixed.move(pointy_label, 600, 380);

    value_label.set_label("Function value: ");
    fixed.add(value_label);
    fixed.move(value_label, 600, 415);

    num_of_iter_label.set_label("Number of iteration: ");
    fixed.add(num_of_iter_label);
    fixed.move(num_of_iter_label, 600, 450);

    fixed.add(button1);
    fixed.move(button1, 20, 450);

    button1.set_label("Calculate");
    button1.signal_button_release_event().connect([&](GdkEventButton*) {
      std::vector<double> coef_vec;
      std::vector<std::pair<double,double>> func_range;

      
      func_range.push_back(std::make_pair(10, 80));
      func_range.push_back(std::make_pair(10, 50));
      func_range.push_back(std::make_pair(0.9, 5));
      func_range.push_back(std::make_pair(0.9, 5));
      

      for(int i = 0; i < function_entry_vec.size(); ++i){
        std::string coef(coef_entry_vec[i].get_text());

        if(coef != ""){
          coef_vec.push_back(std::stod(coef));
        }
      }

      auto f1 = get_f1();
      auto f2 = get_f2();
      auto pen = get_pen();
      std::vector<function> fs = {f1, f2}; 

      std::vector<double> step = {1, 1, 0.1, 0.1};
      FunctionContainer MO_method(fs, func_range, 1e-2, step, pen);
      auto res = MO_method.Convolution(coef_vec);
      for(auto c : res.return_point)
        std::cout << c << " ";
      std::cout << "\n";
      std::cout << pen(res.return_point) <<"\n";

      std::cout << "result f1" << f1(res.return_point) <<"\n";
      std::cout << "result f2" << f2(res.return_point) <<"\n";


      num_of_iter_label.set_label("Number of iteration: " + std::to_string(res.num_iteration));
      pointx_label.set_label("Min at x1: " + std::to_string(res.return_point[0]));
      pointy_label.set_label("Min at x2: " + std::to_string(res.return_point[1]));
      value_label.set_label("Function value: " + std::to_string(res.value));
      
      return true;
    });

    
    
    set_title("Program");
    resize(800, 500);
    show_all();
    for(auto& c : function_entry_vec)
      c.hide();
    for(auto& c : coef_entry_vec)
      c.hide();
    function_entry_vec[0].show();
    coef_entry_vec[0].show();
  }
  
private:
  gtk::Fixed fixed;
  gtk::ScrolledWindow scrolledWindow;
  gtk::Button button1;
  std::vector<gtk::Entry> function_entry_vec;
  std::vector<gtk::Entry> coef_entry_vec;
  std::vector<gtk::Entry> range;
  gtk::Label num_of_iter_label;
  gtk::Label pointx_label;
  gtk::Label pointy_label;
  gtk::Label value_label;
  gtk::Label functions_label;
  gtk::Label params_label;
  gtk::Label range_str_label;
  int num_of_entry = 10;
  int last_entry = 0;
  int button1Clicked = 0;
  int button2Clicked = 0;
};

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv);
  WindowMain window;

  return app->run(window);
}