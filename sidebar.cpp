#include "sidebar.hh"
#include <QFrame>

QString Sidebar::createLabelStyle() {
    return
    "color: rgba(154, 160, 166, " + getTransparency() + ");"
    "font-weight: 700;"
    "padding: 10px;"
    "text-transform: uppercase;"
    "font-weight: 700;"
    "margin-bottom: -5px;";
}

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    isParentMainWindow = (parent && parent->objectName() == "MainWindow");
    qDebug() << "Is parent MainWindow: " << isParentMainWindow;

    QFrame *sidebarFrame = new QFrame(this);

    sidebarFrame->setStyleSheet("background-color: rgba(33, 37, 43, " + getTransparency() + ");");

    sidebarLayout = new QVBoxLayout(this);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    setFixedWidth(250);

    QVBoxLayout *sidebarContentLayout = new QVBoxLayout(sidebarFrame);
    sidebarContentLayout->setContentsMargins(0, 0, 0, 0);
    sidebarContentLayout->setSpacing(0);

    const QString buttonStyle =
        "QPushButton {"
        "text-align: left;"
        "color: rgba(154, 160, 166, " + getTransparency() + ");"
        "background-color: rgba(33, 37, 43, " + getTransparency() + ");"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: rgba(57, 61, 71, " + getTransparency() + ") !important;}"
    ;

    toggleSidebarButton = createButton(" Toggle sidebar", QIcon(":/icons/menu.svg"), buttonStyle);
    sidebarContentLayout->addWidget(toggleSidebarButton);

    connect(toggleSidebarButton, &QPushButton::clicked, this, &Sidebar::toggleSidebar);

    filesLabel = new QLabel("Files", this);
    const QString labelStyle = createLabelStyle();

    filesLabel->setStyleSheet(labelStyle);

    sidebarContentLayout->addWidget(filesLabel);

    allFilesButton = createButton(" All files", QIcon(":/icons/all.svg"), buttonStyle);
    connect(allFilesButton, &QPushButton::clicked, this, &Sidebar::allFilesButtonClicked);
    sidebarContentLayout->addWidget(allFilesButton);

    categoriesLabel = new QLabel("Categories", this);

    categoriesLabel->setStyleSheet(labelStyle);

    sidebarContentLayout->addWidget(categoriesLabel);

    imagesButton = createButton(" Images", QIcon(":/icons/images.svg"), buttonStyle);
    connect(imagesButton, &QPushButton::clicked, this, &Sidebar::imagesButtonClicked);
    sidebarContentLayout->addWidget(imagesButton);

    gifsButton = createButton(" GIFs", QIcon(":/icons/gifs.svg"), buttonStyle);
    connect(gifsButton, &QPushButton::clicked, this, &Sidebar::gifsButtonClicked);
    sidebarContentLayout->addWidget(gifsButton);

    textsButton = createButton(" Texts", QIcon(":/icons/texts.svg"), buttonStyle);
    connect(textsButton, &QPushButton::clicked, this, &Sidebar::textsButtonClicked);
    sidebarContentLayout->addWidget(textsButton);

    videosButton = createButton(" Videos", QIcon(":/icons/videos.svg"), buttonStyle);
    connect(videosButton, &QPushButton::clicked, this, &Sidebar::videosButtonClicked);
    sidebarContentLayout->addWidget(videosButton);

    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    sidebarContentLayout->addItem(spacer);
    sidebarLayout->addWidget(sidebarFrame);
}

QPushButton* Sidebar::createButton(const QString& text, const QIcon& icon, const QString& style)
{
    QPushButton *button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(QSize(22, 22));
    button->setText(text);
    button->setStyleSheet(style);
    button->setFixedSize(250, 40);
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void Sidebar::updateButtonStyles(int targetFolderId) {
    const QString normalStyle =
        "QPushButton {"
        "text-align: left;"
        "color: rgba(154, 160, 166, " + getTransparency() + ");"
        "background-color: rgba(33, 37, 43, " + getTransparency() + ");"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: rgba(57, 61, 71, " + getTransparency() + ") !important;}"
        ;

    const QString selectedStyle =
        "QPushButton {"
        "text-align: left;"
        "color: rgba(154, 160, 166, " + getTransparency() + ");"
        "background-color: rgba(57, 61, 71, " + getTransparency() + ");"
        "border: none;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 700;}"
        "QPushButton:hover {"
        "background-color: rgba(90, 94, 106, " + getTransparency() + ") !important;}"
        ;

    allFilesButton->setStyleSheet(targetFolderId == 0 ? selectedStyle : normalStyle);
    imagesButton->setStyleSheet(targetFolderId == 1 ? selectedStyle : normalStyle);
    gifsButton->setStyleSheet(targetFolderId == 2 ? selectedStyle : normalStyle);
    textsButton->setStyleSheet(targetFolderId == 3 ? selectedStyle : normalStyle);
    videosButton->setStyleSheet(targetFolderId == 4 ? selectedStyle : normalStyle);
}

QString Sidebar::getTransparency() const {
    return isParentMainWindow ? "1.0" : "0.5";
}

void Sidebar::toggleSidebar() {
    sideBarMaximized = !sideBarMaximized;

    if (!sideBarMaximized) {
        allFilesButton->setText("");
        imagesButton->setText("");
        gifsButton->setText("");
        textsButton->setText("");
        videosButton->setText("");
        toggleSidebarButton->setText("");
        this->setFixedWidth(44);
        categoriesLabel->hide();
        filesLabel->hide();
    } else {
        allFilesButton->setText(" All files");
        imagesButton->setText(" Images");
        gifsButton->setText(" GIFs");
        textsButton->setText(" Texts");
        videosButton->setText(" Videos");
        toggleSidebarButton->setText(" Toggle sidebar");
        this->setFixedWidth(250);
        categoriesLabel->show();
        filesLabel->show();
    }
}

