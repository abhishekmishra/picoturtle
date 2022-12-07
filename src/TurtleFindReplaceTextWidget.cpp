#include "TurtleFindReplaceTextWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

using namespace turtle;

TurtleFindReplaceTextWidget::TurtleFindReplaceTextWidget(QWidget* parent)
	: QWidget(parent),
	replace_enabled{ false }
{
	auto vbox = new QVBoxLayout();
	auto find_widget = new QWidget();
	auto find_hbox = new QHBoxLayout();

	auto find_label = new QLabel("Find:");
	find_label->setContentsMargins(0, 0, 0, 0);
	auto find_text = new QLineEdit();
	find_text->setContentsMargins(0, 0, 0, 0);
	find_hbox->addWidget(find_label);
	find_hbox->addWidget(find_text);

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
};

bool TurtleFindReplaceTextWidget::is_replace_enabled() 
{
	return replace_enabled;
}

void TurtleFindReplaceTextWidget::set_replace_enabled(bool flag) 
{
	this->replace_enabled = flag;
}
