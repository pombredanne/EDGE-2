// cooper king

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

// A basic shell of a game object.
class GameObject
{
public:
  explicit GameObject(chaiscript::ChaiScript &chai, const std::string& scriptFile)
  {
    // Load up script object
    static int i = 0;
    i++;

    const std::string obj_id = "obj_" + std::to_string(i);

    chai.eval_file(scriptFile);
    chai.eval("auto " + obj_id + " = script_object();");

    m_onStart  = chai.eval<std::function<void()> >("fun() {" + obj_id + ".on_start(); }");  
    m_onUpdate = chai.eval<std::function<void(float)> >("fun(dt) {" + obj_id + ".on_update(dt); }");
    m_onEnd    = chai.eval<std::function<void()> >("fun() {" + obj_id + ".on_end(); }");
  }

  // Game object hooks.

  void onStart() {
    m_onStart();
  }

  void onUpdate(const float dt) {
    m_onUpdate(dt);
  }

  void onEnd() {
    m_onEnd();
  }

private:

  // Calling the chai methods.
  std::function<void()>             m_onStart;
  std::function<void(const float)>  m_onUpdate;
  std::function<void()>             m_onEnd;

};

#include <iostream>

std::string say(const std::string &text)
{
  std::cout << "says: " << text << std::endl;
  return text;
}

int main()
{
  chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
  chai.add(chaiscript::fun(&say), "say");

  try {
    chai.eval("say(\"hello world\");");    
  }
  catch(const chaiscript::exception::eval_error &e) {
    std::cout << e.pretty_print() << std::endl;
  }
  catch(...) {
  }

  GameObject obj(chai, "object.chai");
  
  obj.onStart();
  obj.onUpdate(0.16f);
  obj.onEnd();

  return 0;
}