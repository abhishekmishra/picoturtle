#include "TurtleFindReplaceTextWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QMessageBox>

using namespace turtle;

TurtleFindReplaceTextWidget::TurtleFindReplaceTextWidget(TurtleCodeEditorTextWidget *ed, QWidget *parent)
	: QWidget(parent),
	  replace_enabled{false},
	  editor{ed}
{
	auto vbox = new QVBoxLayout();
	auto find_widget = new QWidget();
	auto find_hbox = new QHBoxLayout();
	replace_widget = new QWidget();
	auto replace_hbox = new QHBoxLayout();

	auto find_label = new QLabel("Find:");
	find_label->setContentsMargins(0, 0, 0, 0);
	auto find_lineedit = new QLineEdit();
	find_lineedit->setContentsMargins(0, 0, 0, 0);
	auto find_next_btn = new QPushButton("Next");
	auto find_previous_btn = new QPushButton("Previous");
	auto close_btn = new QPushButton("X");
	find_hbox->addWidget(find_label);
	find_hbox->addWidget(find_lineedit);
	find_hbox->addWidget(find_previous_btn);
	find_hbox->addWidget(find_next_btn);
	find_hbox->addWidget(close_btn);

	find_widget->setLayout(find_hbox);
	find_widget->setContentsMargins(0, 0, 0, 0);
	find_hbox->setContentsMargins(0, 0, 0, 0);
	find_hbox->setSpacing(0);

	auto replace_label = new QLabel("Replace");
	auto replace_lineedit = new QLineEdit();
	auto replace_next_btn = new QPushButton("Next");
	auto replace_all_btn = new QPushButton("All");
	replace_hbox->addWidget(replace_label);
	replace_hbox->addWidget(replace_lineedit);
	replace_hbox->addWidget(replace_next_btn);
	replace_hbox->addWidget(replace_all_btn);
	replace_widget->setLayout(replace_hbox);
	replace_widget->setContentsMargins(0, 0, 0, 0);
	replace_hbox->setContentsMargins(0, 0, 0, 0);
	replace_hbox->setSpacing(0);

	vbox->setSpacing(0);
	vbox->setContentsMargins(0, 0, 0, 0);
	vbox->addWidget(find_widget);
	vbox->addWidget(replace_widget);

	if (!is_replace_enabled())
	{
		replace_widget->hide();
	}

	setLayout(vbox);

	setMinimumHeight(50);
	setMinimumWidth(300);
	setContentsMargins(0, 0, 0, 0);

	connect(find_next_btn, &QPushButton::clicked, [=]()
			{ find_next(find_lineedit->text()); });

	connect(find_previous_btn, &QPushButton::clicked, [=]()
			{ find_previous(find_lineedit->text()); });

	connect(replace_next_btn, &QPushButton::clicked, [=]()
			{ replace_next(find_lineedit->text(), replace_lineedit->text()); });

	connect(replace_all_btn, &QPushButton::clicked, [=]()
			{ replace_all(find_lineedit->text(), replace_lineedit->text()); });

	connect(close_btn, &QPushButton::clicked, [=]()
			{ hide(); });
};

bool TurtleFindReplaceTextWidget::is_replace_enabled()
{
	return replace_enabled;
}

void TurtleFindReplaceTextWidget::set_replace_enabled(bool flag)
{
	this->replace_enabled = flag;
	if (is_replace_enabled())
	{
		replace_widget->show();
	}
	else
	{
		replace_widget->hide();
	}
}

void TurtleFindReplaceTextWidget::find_next(QString text)
{
	bool found = editor->find(text);
	if (!found)
	{
		QMessageBox::information(this, "Find Next", "No further matches.");
	}
}

void TurtleFindReplaceTextWidget::find_previous(QString text)
{
	QTextDocument::FindFlags option;
	option |= QTextDocument::FindBackward;
	bool found = editor->find(text, option);
	if (!found)
	{
		QMessageBox::information(this, "Find Previous", "No further matches.");
	}
}

void TurtleFindReplaceTextWidget::replace_next(QString text, QString replace_text)
{
	bool found = editor->find(text);
	if (found)
	{
		editor->insertPlainText(replace_text);
	}
	if (!found)
	{
		QMessageBox::information(this, "Replace Next", "No further matches.");
	}
}

void TurtleFindReplaceTextWidget::replace_all(QString text, QString replace_text)
{
	int count = 0;

	// move to beginning of editor
	QTextCursor textCursor = editor->textCursor();
	textCursor.movePosition(QTextCursor::Start);
	editor->setTextCursor(textCursor);

	while (editor->find(text))
	{
		editor->insertPlainText(replace_text);
		count += 1;
	}
	if (count == 0)
	{
		QMessageBox::information(this, "Replace All", "No matches found.");
	}
	else
	{
		QMessageBox::information(this, "Replace All", QString::number(count) + " matches replaced.");
	}
}
