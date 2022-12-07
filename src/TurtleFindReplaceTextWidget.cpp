#include "TurtleFindReplaceTextWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>

using namespace turtle;

TurtleFindReplaceTextWidget::TurtleFindReplaceTextWidget(TurtleCodeEditorTextWidget* ed, QWidget* parent)
	: QWidget(parent),
	replace_enabled{ false },
	editor{ ed }
{
	auto vbox = new QVBoxLayout();
	auto find_widget = new QWidget();
	auto find_hbox = new QHBoxLayout();

	auto find_label = new QLabel("Find:");
	find_label->setContentsMargins(0, 0, 0, 0);
	auto find_text = new QLineEdit();
	find_text->setContentsMargins(0, 0, 0, 0);
	auto find_next_btn = new QPushButton("Next");
	auto find_previous_btn = new QPushButton("Previous");
	auto close_btn = new QPushButton("X");
	find_hbox->addWidget(find_label);
	find_hbox->addWidget(find_text);
	find_hbox->addWidget(find_previous_btn);
	find_hbox->addWidget(find_next_btn);
	find_hbox->addWidget(close_btn);

	find_widget->setLayout(find_hbox);
	find_widget->setContentsMargins(0, 0, 0, 0);
	find_hbox->setContentsMargins(0, 0, 0, 0);
	find_hbox->setSpacing(0);

	vbox->setSpacing(0);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->addWidget(find_widget);
	setLayout(vbox);

	setMinimumHeight(50);
	setMinimumWidth(300);
	setContentsMargins(0, 0, 0, 0);

	connect(find_next_btn, &QPushButton::clicked, [=]() {
		editor->find(find_text->text());
		});

	connect(find_previous_btn, &QPushButton::clicked, [=]() {
		QTextDocument::FindFlags option;
		option |= QTextDocument::FindBackward;
		editor->find(find_text->text(), option);
		});
};

bool TurtleFindReplaceTextWidget::is_replace_enabled()
{
	return replace_enabled;
}

void TurtleFindReplaceTextWidget::set_replace_enabled(bool flag)
{
	this->replace_enabled = flag;
}
