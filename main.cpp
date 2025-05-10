#include <vector>
#include <iostream>
#include "window.h"
#include "helpers.h"
#include "registration.h"
#include "settingsstorage.h"

//
// consts
//

// registration storage types
static const std::string GRAPHICS = "graphics";
static const std::string BUTTONS = "buttons";

//!
//! \brief The IGraphicItem class - graphic item interface
//!

class IGraphicItem
{
public:
    //!
    //! ShapeType - unique graphic type id
    //!

    using ShapeType = uint32_t;

    using Shared = std::shared_ptr<IGraphicItem>;

    virtual void draw() = 0;

protected:
    simple_pair<int32_t> center {0, 0};

};

//!
//! \brief The SceneModel class - data item storage
//!

class SceneModel
{
public:
    using Pointer = std::weak_ptr<SceneModel>;
    using Shared = std::shared_ptr<SceneModel>;

    SceneModel()
    {
        std::printf("application scene model created\n");
    }

    //!
    //! \brief loadSceneData - load graphic data to data model (load data from existing document)
    //!

    void loadSceneData(/* some info */)
    {
        std::printf("load graphic scene data to model\n");
    }

    //!
    //! \brief newScene - create new empty data scene (new empty document)
    //!

    void newScene()
    {
        std::printf("make new empty graphic scene\n");
    }

    //!
    //! \brief saveToFile - save scene data to storage (save document)
    //!

    void saveToFile(/* some info / file name */)
    {
        std::printf("save graphic scene data from model to file\n");
    }

    //!
    //! \brief addShape - add graphic item to data model
    //! \param item - graphic item descriptor
    //!

    void addShape(const IGraphicItem::Shared &item)
    {
        std::printf("add graphic item to data model\n");
        m_graphicItems.push_back(item);
        item->draw();
    }

    //!
    //! \brief delCurrentItem - remove current selected item from scene
    //!

    void delCurrentItem()
    {
        std::printf("remove graphic item from data model\n");
    }

private:
    std::vector<IGraphicItem::Shared> m_graphicItems;
};

//!
//! \brief The IToolButton class - base button class
//!

class IToolButton : public IButtonInfo
{
public:
    using Shared = std::shared_ptr<IToolButton>;

    virtual void onClick(const SceneModel::Pointer &model) = 0;

protected:

    //!
    //! \brief takeModel - helper for data model access
    //! \param model
    //! \param sceneModel
    //! \return
    //!

    bool takeModel(const SceneModel::Pointer &model, SceneModel::Shared &sceneModel)
    {
        sceneModel = model.lock();
        return (sceneModel != nullptr);
    }

};

//!
//! \brief The GraphicItemCreator class - graphic item (shape) creator
//!

struct GraphicItemCreator : public CreateItemService
{
    //!
    //! GraphicItemCreator - creator
    //!

    template<typename T>
    static CreateItemService::Shared getCreator()
    {
        //! create GraphicItemCreator class
        return CreateItemService::Shared(new GraphicItemCreator);
    }

    //!
    //! find and create graphic item of required type from registred items
    //!

    template<typename T, typename...Ts>
    IGraphicItem::Shared createInstance(Ts&&... /*params*/) const
    {
        return IGraphicItem::Shared();
    }

};

//!
//! \brief The ButtonItemCreator class - UI buttons creator
//!

struct ButtonItemCreator : public CreateItemService
{
    //!
    //! ButtonItemCreator - creator
    //!

    template<typename T, typename...Ts>
    static CreateItemService::Shared getCreator(Ts&&... /*params*/)
    {
        return CreateItemService::Shared(new ButtonItemCreator);
    }

    //!
    //! ButtonItemCreator - creator
    //!

    template<typename T>
    static CreateItemService::Shared getCreator()
    {

        return CreateItemService::Shared(new ButtonItemCreator);
    }

    //!
    //! find and create UI button of required type from registred items
    //!

    template<typename T, typename...Ts>
    IToolButton::Shared createInstance(Ts&&... /*params*/) const
    {
        return IToolButton::Shared();
    }

};

//!
//! \brief createGraphicItemFabrique - graphic item (shape) creating fabrique
//! \param type - unique graphic item id
//! \param graphicItem - created graphic item instance
//! \return true on success
//!

bool createGraphicItemFabrique(const IGraphicItem::ShapeType &type, IGraphicItem::Shared &graphicItem)
{
    const auto registrator = Registrator::takeStorage(GRAPHICS);
    if (registrator) {
        const auto creator = dynamic_cast<GraphicItemCreator *>(registrator.get());
        if (creator != nullptr) {
            graphicItem = creator->createInstance<IGraphicItem>(type);
            return true;
        }
    }
    return false;
}

//!
//! \brief createButtonFabrique - UI button creating fabrique
//! \param type - unique button id
//! \param buttonItem - created button instance
//! \return true on success
//!

bool createButtonFabrique(const IButtonInfo::Type &type, IToolButton::Shared &buttonItem)
{
    const auto registrator = Registrator::takeStorage(BUTTONS);
    if (registrator) {
        const auto creator = std::dynamic_pointer_cast<const ButtonItemCreator>(registrator);
        if (creator != nullptr) {
            buttonItem = creator->createInstance<IToolButton>(type);
            return true;
        }
    }
    return false;
}

//!
//! \brief The Rectangle class - presentation of rectangle shape
//!

struct Rectangle : public IGraphicItem
{
    //!
    //! \brief Id - unique graphic item (rectangle) id
    //!

    static const ShapeType Id;

    //!
    //! \brief draw - draw rectangle graphic item with defined width, height and position
    //!

    void draw() override
    {
        std::printf("draw rectangle at point %s, dimensions %s\n", print_pair(center, ",").c_str(), print_pair(dimensions, "x").c_str());
    }

private:

    //!
    //! @var dimensions - width and height of rectangle, mm
    //!

    simple_pair<uint32_t> dimensions {0, 0};

};
const Rectangle::ShapeType Rectangle::Id {1};

//!
//! \brief The Square class - presentation of square shape
//!

struct Square : public IGraphicItem
{
    //!
    //! \brief Id - unique graphic item (square) id
    //!

    static const ShapeType Id;

    //!
    //! \brief draw - draw square graphic item with defined side and position
    //!

    void draw() override
    {
        std::printf("draw square at point %s, with side %s\n", print_pair(center, ",").c_str(), to_str(side).c_str());
    }

private:

    //!
    //! @var side - square side, mm
    //!
    //!
    uint32_t side {0};

};
const Square::ShapeType Square::Id {2};

//!
//! \brief The Сircle class - presentation of circle shape
//!

struct Сircle : public IGraphicItem
{
    //!
    //! \brief Id  - unique graphic item (circle) id
    //!

    static const ShapeType Id;

    //!
    //! \brief draw - draw circle graphic item with defined radius and position
    //!

    void draw() override
    {
        std::printf("draw circle at point %s, with radius %s\n", print_pair(center, ",").c_str(), to_str(radius).c_str());
    }

private:

    //!
    //! @var radius - radius of circle, mm
    //!

    uint32_t radius {0};

};
const Сircle::ShapeType Сircle::Id {3};

//!
//! \brief The SceneView class - place for drawing items
//!

class SceneView : public Window
{
public:
    SceneView(SceneModel::Pointer model) :
        m_dataModel(model)
    {
        std::printf("application scene view created\n");
    }

private:
    SceneModel::Pointer m_dataModel;

};

//!
//! \brief The OpenFileButton class - button for load data from file
//!

class OpenFileButton : public IToolButton
{
public:

    //!
    //! \brief Id - unique UI button (open file button) id
    //!

    static const Type Id {};

    //!
    //! \brief onClick - handler for load data from file to model
    //! \param model - scene data model
    //!

    void onClick(const SceneModel::Pointer &model) override
    {
        std::printf("open graphics file");
        SceneModel::Shared sceneModel;
        if (takeModel(model, sceneModel)) {
            sceneModel->loadSceneData(/* some info */);
        }
    }

};

//!
//! \brief The NewFileButton class - button for new scene/document creation
//!

class NewFileButton : public IToolButton
{
public:

    //!
    //! \brief Id - unique UI button (new file button) id
    //!

    static const Type Id {};

    //!
    //! \brief onClick - handler for new document creating
    //! \param model - scene data model
    //!

    void onClick(const SceneModel::Pointer &model) override
    {
        std::printf("new graphics file");
        SceneModel::Shared sceneModel;
        if (takeModel(model, sceneModel)) {
            sceneModel->newScene();
        }
    }

};

//!
//! \brief The SaveFileButton class - button for save scene data to file
//!

class SaveFileButton : public IToolButton
{
public:

    //!
    //! \brief Id - unique UI button (save to file button) id
    //!

    static const Type Id {};

    //!
    //! \brief onClick - handler for save data to file
    //! \param model - scene data model
    //!

    void onClick(const SceneModel::Pointer &model) override
    {
        std::printf("save graphic scene data to file");
        SceneModel::Shared sceneModel;
        if (takeModel(model, sceneModel)) {
            sceneModel->saveToFile(/* some info / file name */);
        }
    }

};

//!
//! \brief The AddShapeButton class - button for append graphic items (shapes) to scene
//!

class AddShapeButton : public IToolButton
{
public:

    //!
    //! \brief Id - unique UI button (add shape button) id
    //!

    static const Type Id {};

    AddShapeButton(const IGraphicItem::ShapeType &type) :
        IToolButton(),
        m_shapeType(type)
    {
    }

    //!
    //! \brief onClick - handler for new graphic item creation
    //! \param model - scene data model
    //!

    void onClick(const SceneModel::Pointer &model) override
    {
        SceneModel::Shared sceneModel;
        if (takeModel(model, sceneModel)) {
            IGraphicItem::Shared graphicItem;
            if (createGraphicItemFabrique(m_shapeType, graphicItem))
                sceneModel->addShape(graphicItem);
        }
    }

private:
    IGraphicItem::ShapeType m_shapeType;

};

//!
//! \brief The DelShapeButton class - button for delete current selected graphic item
//!

class DelShapeButton : public IToolButton
{
public:

    //!
    //! \brief Id - unique UI button (delete current graphic item button) id
    //!

    static const Type Id {};

    //!
    //! \brief onClick - handler for current selected graphic item deletion
    //! \param model - scene data model
    //!

    void onClick(const SceneModel::Pointer &model) override
    {
        SceneModel::Shared sceneModel;
        if (takeModel(model, sceneModel)) {
            sceneModel->delCurrentItem();
        }
    }

};

//!
//! \brief The ToolsBar class - tools bar for graphic items selection and other operation
//!

class ToolsBar : public Window
{
public:

    //!
    //! \brief ToolsBar - tools bar window class constructor
    //! \param model - scene data model
    //!

    ToolsBar(SceneModel::Pointer model) :
        m_dataModel(model)
    {
        const auto &buttonsIdsList = SettingStorage::instance().viewButtons();
        for (const auto &buttonId: buttonsIdsList) {
            IToolButton::Shared buttonItem;
            if (createButtonFabrique(buttonId, buttonItem)) {
                m_buttons.push_back(buttonItem);
            }
        }
        std::printf("application tools bar created\n");
    }

private:
    std::vector<IToolButton::Shared> m_buttons;
    SceneModel::Pointer m_dataModel;

};

//
// registration
//

// graphis items reg
static const auto regCircle = Registrator::regCreator(GRAPHICS, GraphicItemCreator::getCreator<Сircle>());
static const auto regSquare = Registrator::regCreator(GRAPHICS, GraphicItemCreator::getCreator<Square>());
static const auto regRectangle = Registrator::regCreator(GRAPHICS, GraphicItemCreator::getCreator<Rectangle>());

// UI functionality buttons reg
static const auto regNewFile = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<NewFileButton>());
static const auto regOpenFile = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<OpenFileButton>());
static const auto regSaveFile = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<SaveFileButton>());
static const auto regDoCircle = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<AddShapeButton>(Сircle::Id));
static const auto regDoSquare = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<AddShapeButton>(Square::Id));
static const auto regDoRectangle = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<AddShapeButton>(Rectangle::Id));
static const auto regDelShape = Registrator::regCreator(BUTTONS, ButtonItemCreator::getCreator<DelShapeButton>());

//!
//! \brief The MainWindow class - main window class
//!

class MainWindow
{
public:

    //!
    //! \brief MainWindow - main window clacc constructor
    //!

    MainWindow()
    {
        m_sceneModel = std::make_shared<SceneModel>();
        m_toolsBar = std::make_shared<ToolsBar>(m_sceneModel);
        m_sceneView = std::make_shared<SceneView>(m_sceneModel);
        std::printf("main application window created\n");

    }

    //!
    //! \brief exec - run application execution
    //! \return
    //!

    void exec()
    {
        std::printf("application work\n");
    }

private:
    std::shared_ptr<ToolsBar> m_toolsBar;
    std::shared_ptr<SceneModel> m_sceneModel;
    std::shared_ptr<SceneView> m_sceneView;

};

//!
//! \brief main - main project function
//!

int main()
{
    std::cout << "Home work #5" << std::endl;

    std::printf("start application\n");
    MainWindow mw;

    // run GUI
    mw.exec();

    // finaly
    std::printf("application closed\n");

    return 0;
}
