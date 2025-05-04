#include <iostream>
#include <memory>

//!
//! \brief The Scene class - place for drawing items
//!

class Scene
{
public:
    Scene()
    {

    }

};

//!
//! \brief The MainWindow class - main window class
//!

class MainWindow
{
public:
    MainWindow()
    {
        m_scene = std::make_shared<Scene>();

    }

    //!
    //! \brief exec -
    //! \return
    //!

    void exec()
    {
        std::printf("application work\n");
    }

private:
    std::shared_ptr<Scene> m_scene;


};

//!
//! \brief main - main project function
//!

int main()
{
//    std::cout << "Home work #5" << std::endl;

    std::printf("start application\n");
    MainWindow mw;

    // run GUI
    mw.exec();

    //
    std::printf("application closed\n");

    return 0;
}
