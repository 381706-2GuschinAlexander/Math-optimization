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
    int h_y = 55;

    img = std::vector<gtk::Image>(num_of_entry);
    coef_entry_vec = std::vector<gtk::Entry>(num_of_entry);
    pointx = std::vector<gtk::Label>(4);


    for(int i = 0; i < num_of_entry; ++i){
      fixed.add(coef_entry_vec[i]);
      fixed.move(coef_entry_vec[i], 400, start_y + i * h_y);
      coef_entry_vec[i].set_text("1");


      img[i].set(i == 0 ? "f1.png" : "f2.png");
      fixed.add(img[i]);
      fixed.move(img[i], 20, start_y + i * h_y);
    }

    functions_label.set_label("f(x)");
    fixed.add(functions_label);
    fixed.move(functions_label, 125, 10);

    params_label.set_label("Weight");
    fixed.add(params_label);
    fixed.move(params_label, 400, 10);

    for(uint32_t i = 0; i < 4; ++i){
      char buf[40];
      sprintf(buf, "Min at x%u", i);

      pointx[i].set_label(buf);
      fixed.add(pointx[i]);
      fixed.move(pointx[i], 20, 205 + 35 * i);
    }
    
    value_label1.set_label("f1 value: ");
    fixed.add(value_label1);
    fixed.move(value_label1, 20, 345);

    value_label2.set_label("f2 value: ");
    fixed.add(value_label2);
    fixed.move(value_label2, 20, 380);

    // num_of_iter_label.set_label("Number of iteration: ");
    // fixed.add(num_of_iter_label);
    // fixed.move(num_of_iter_label, 600, 450);

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
      

      for(int i = 0; i < num_of_entry; ++i){
        std::string coef(coef_entry_vec[i].get_text());

        if(coef != ""){
          coef_vec.push_back(std::stod(coef));
        }
      }

      auto f1 = get_f1();
      auto f2 = get_f2();
      auto pen = get_pen();
      std::vector<function> fs = {f1, f2}; 

      std::vector<double> step = {1, 2, 0.1, 0.1};
      FunctionContainer MO_method(fs, func_range, 1e-2, step, pen);
      auto res = MO_method.Convolution(coef_vec);
      for(auto c : res.return_point)
        std::cout << c << " ";
      std::cout << "\n";
      std::cout <<"penalty " <<pen(res.return_point) <<"\n";

      std::cout << "result f1 " << f1(res.return_point) <<"\n";
      std::cout << "result f2 " << f2(res.return_point) <<"\n";


      //num_of_iter_label.set_label("Number of iteration: " + std::to_string(res.num_iteration));
      
      pointx[0].set_label("Min at x1: " + std::to_string(res.return_point[0]));
      pointx[1].set_label("Min at x2: " + std::to_string(res.return_point[1]));
      pointx[2].set_label("Min at x3: " + std::to_string(res.return_point[2]));
      pointx[3].set_label("Min at x4: " + std::to_string(res.return_point[3]));

      value_label1.set_label("f1 value: " + std::to_string(f1(res.return_point)));
      value_label2.set_label("f2 value: " + std::to_string(f2(res.return_point)));
      
      return true;
    });

    
    
    set_title("Program");
    resize(600, 500);
    show_all();
  }
  
private:
  gtk::Fixed fixed;
  gtk::ScrolledWindow scrolledWindow;
  gtk::Button button1;
  std::vector<gtk::Image> img;
  std::vector<gtk::Entry> coef_entry_vec;
  std::vector<gtk::Entry> range;
  std::vector<gtk::Label> pointx;
  gtk::Label num_of_iter_label;

  gtk::Label value_label1;
  gtk::Label value_label2;
  gtk::Label functions_label;
  gtk::Label params_label;

  int num_of_entry = 2;
  int last_entry = 0;
  int button1Clicked = 0;
  int button2Clicked = 0;
};

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv);
  WindowMain window;

  return app->run(window);
}