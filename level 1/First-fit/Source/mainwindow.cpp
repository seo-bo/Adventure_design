#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include "sub.h"
#include <QInputDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

Node* root = NULL;
Heap* min_heap = make_heap();
Chunk* head = NULL;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	resize(1080, 720);

	this->setStyleSheet("background-color: rgb(170,170,170);");

	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

	QGraphicsView* graphicsView = new QGraphicsView(this);
	QGraphicsScene* scene = new QGraphicsScene(this);
	graphicsView->setScene(scene);
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QString buttonStyle = "QPushButton {"
		"background-color: white;"
		"border-radius: 10px;"
		"border: 2px solid black;"
		"font-size: 16px;"
		"font-weight: bold;"
		"color: black;"
		"padding: 10px;"
		"}";

	QPushButton* expandButton = new QPushButton("크기 늘리기", this);
	expandButton->setStyleSheet(buttonStyle);

	QPushButton* shrinkButton = new QPushButton("크기 줄이기", this);
	shrinkButton->setStyleSheet(buttonStyle);

	QPushButton* mergeButton = new QPushButton("머지 소트", this);
	mergeButton->setStyleSheet(buttonStyle);

	QPushButton* insertButton = new QPushButton("노드 삽입", this);
	insertButton->setStyleSheet(buttonStyle);

	QPushButton* deleteButton = new QPushButton("노드 삭제", this);
	deleteButton->setStyleSheet(buttonStyle);

	QPushButton* printButton = new QPushButton("출력", this);
	printButton->setStyleSheet(buttonStyle);

	QPushButton* clearButton = new QPushButton("초기화", this);
	clearButton->setStyleSheet(buttonStyle);

	buttonLayout->addWidget(expandButton);
	buttonLayout->addWidget(shrinkButton);
	buttonLayout->addWidget(mergeButton);
	buttonLayout->addWidget(insertButton);
	buttonLayout->addWidget(deleteButton);
	buttonLayout->addWidget(printButton);
	buttonLayout->addWidget(clearButton);

	mainLayout->addWidget(graphicsView);
	mainLayout->addLayout(buttonLayout);

	connect(expandButton, &QPushButton::clicked, this, [=]() {
		bool ok;
		QInputDialog inputDialog;
		inputDialog.setStyleSheet("QLineEdit { color: black; } QDialog { color: black; } QPushButton { color: black; }");
		int size = inputDialog.getInt(this, "크기 늘리기", "사이즈를 입력하세요:", 0, 0, 100000, 1, &ok);
		if (ok && size > 0)
		{
			expand(&head, min_heap, size);
			scene->clear();
			drawChunks(scene);
		}
		});


	connect(shrinkButton, &QPushButton::clicked, this, [=]()
		{
			bool ok;
			QInputDialog inputDialog;
			inputDialog.setStyleSheet("QLineEdit { color: black; } QDialog { color: black; } QPushButton { color: black; }");
			int size = inputDialog.getInt(this, "크기 줄이기", "사이즈를 입력하세요:", 0, 0, 100000, 1, &ok);
			if (ok && size > 0)
			{
				shrink(&head, min_heap, size);
				scene->clear();
				drawChunks(scene);
			}
		});

	connect(mergeButton, &QPushButton::clicked, this, [=]()
		{
			merge_sort(&root, &head, min_heap);
			scene->clear();
			drawChunks(scene);
		});

	connect(insertButton, &QPushButton::clicked, this, [=]()
		{
			bool ok;
			QInputDialog inputDialog;
			inputDialog.setStyleSheet("QLineEdit { color: black; } QDialog { color: black; } QPushButton { color: black; }");
			QString nodeName = inputDialog.getText(this, "노드 삽입", "노드 이름을 입력하세요:", QLineEdit::Normal, "", &ok);
			std::string tempName = nodeName.toStdString();
			char* name = _strdup(tempName.c_str());

			if (ok && !nodeName.isEmpty() && search(root, name) == NULL)
			{
				int size = inputDialog.getInt(this, "노드 삽입", "사이즈를 입력하세요:", 0, 0, 100000, 1, &ok);
				if (ok && size > 0)
				{
					Chunk* temp = my_alloc(&head, min_heap, size);
					if (temp != NULL)
					{
						root = insert(root, name, temp);
						scene->clear();
						drawChunks(scene);
					}
				}
			}
			else {
				free(name);
			}
		});

	connect(deleteButton, &QPushButton::clicked, this, [=]() {
		bool ok;
		QInputDialog inputDialog;
		inputDialog.setStyleSheet("QLineEdit { color: black; } QDialog { color: black; } QPushButton { color: black; }");
		QString nodeName = inputDialog.getText(this, "노드 삭제", "삭제할 노드 이름을 입력하세요:", QLineEdit::Normal, "", &ok);
		if (ok && !nodeName.isEmpty())
		{
			std::string tempName = nodeName.toStdString();
			char* name = _strdup(tempName.c_str());

			Node* pivot = search(root, name);
			if (pivot != NULL)
			{
				my_free(min_heap, pivot->address);
				root = del(root, name);
				scene->clear();
				drawChunks(scene);
			}
		}
		});

	connect(printButton, &QPushButton::clicked, this, [=]()
		{
			scene->clear();
			drawChunks(scene);
		});

	connect(clearButton, &QPushButton::clicked, this, [=]()
		{
			clear();
			scene->clear();
		});
}

MainWindow::~MainWindow() {}

void MainWindow::drawChunks(QGraphicsScene* scene)
{
	if (head == NULL)
	{
		return;
	}

	Chunk* now = head;
	int x = 50;
	int y = 50;

	do
	{
		QBrush brush;
		QPen pen(Qt::gray);
		QColor textColor;
		QString chunkText = QString("start: %1\nend: %2\nsize: %3\nused: %4")
			.arg(now->start)
			.arg(now->end)
			.arg(now->size)
			.arg(now->used ? "true" : "false");

		if (now->used)
		{
			brush.setColor(QColor(173, 216, 230));
			brush.setStyle(Qt::SolidPattern);
			textColor = Qt::white;
		}
		else
		{
			brush.setColor(Qt::white);
			brush.setStyle(Qt::SolidPattern);
			textColor = Qt::black;
		}

		QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, 150, 100);
		rect->setBrush(brush);
		rect->setPen(pen);
		scene->addItem(rect);

		QGraphicsTextItem* text = new QGraphicsTextItem(chunkText);
		text->setPos(x + 5, y + 5);
		text->setDefaultTextColor(textColor);
		QFont font("Arial", 12);
		text->setFont(font);
		scene->addItem(text);

		if (now->right != head)
		{
			QGraphicsLineItem* line = new QGraphicsLineItem(x + 150, y + 50, x + 200, y + 50);
			scene->addItem(line);
		}

		now = now->right;
		x += 200;
	} while (now != head);
}
