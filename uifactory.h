#pragma once

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QCheckBox>
#include <QSize>
#include <QPixmap>

#define DefineMacro(classRet,name,type)               \
   classRet& set##name(type _arg)                     \
   {							                      \
		name = _arg;                                  \
        return *this;                                 \
   }                                                  \
                                                      \
   type get##name() const                             \
   {                                                  \
     return this->name;                               \
   }


class ControlPorperty
{
public:
	explicit ControlPorperty() {}
	~ControlPorperty() {}

	DefineMacro(ControlPorperty, Text, QString);
	DefineMacro(ControlPorperty, Objectname, QString);
	DefineMacro(ControlPorperty, Texts, QStringList);
	DefineMacro(ControlPorperty, Min, int);
	DefineMacro(ControlPorperty, Max, int);
	DefineMacro(ControlPorperty, Value, int);
	DefineMacro(ControlPorperty, Orientation, Qt::Orientation);
	DefineMacro(ControlPorperty, Size, QSize);
	DefineMacro(ControlPorperty, Icon, QString);
	DefineMacro(ControlPorperty, Color, QColor);
	DefineMacro(ControlPorperty, ToolTips, QString);
	DefineMacro(ControlPorperty, Icons, QStringList);
	DefineMacro(ControlPorperty, Stylesheet, QString);
	DefineMacro(ControlPorperty, EnabledState, bool);
	DefineMacro(ControlPorperty, ReadOnlyState, bool);
	DefineMacro(ControlPorperty, CheckableState, bool);
	DefineMacro(ControlPorperty, Alignment, Qt::Alignment);


	ControlPorperty& init()
	{
		Text.clear();
		Objectname.clear();
		Texts.clear();
		Min = 0;
		Max = 0;
		Value = 0;
		Orientation = Qt::Horizontal;
		Icon.clear();
		Size = QSize(24, 24);
		Color = QColor(0, 0, 0);
		ToolTips.clear();
		Icons.clear();
		Stylesheet.clear();

		EnabledState = true;
		ReadOnlyState = false;
		CheckableState = false;

		Alignment = Qt::AlignLeft;

		return *this;
	}

private:
	QString            Text;
	QStringList        Texts;

	QString            Icon;
	QStringList        Icons;

	QString            Objectname;
	int                Min;
	int                Max;
	int                Value;
	Qt::Orientation    Orientation;
	QSize              Size;
	QColor             Color;
	QString            ToolTips;
	QString            Stylesheet;

	bool               EnabledState;
	bool               ReadOnlyState;
	bool               CheckableState;

	Qt::Alignment      Alignment;
};

namespace UiFactory
{
	QWidget* createComboBox(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createPushButton(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createLabel(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createLineEdit(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createSlider(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createCheckBox(const ControlPorperty& property, QWidget* parent = nullptr);
	QWidget* createSpinBox(const ControlPorperty& property, QWidget* parent = nullptr);

	template<typename T>
	T* createControl(const ControlPorperty& property, QWidget* parent = nullptr)
	{
		typedef QWidget* (*Function)(const ControlPorperty& property, QWidget* parent);
		static QMap<QString, Function> nameToFunc;
		if (nameToFunc.isEmpty())
		{
			nameToFunc["QComboBox"] = createComboBox;
			nameToFunc["QPushButton"] = createPushButton;
			nameToFunc["QLabel"] = createLabel;
			nameToFunc["QLineEdit"] = createLineEdit;
			nameToFunc["QSlider"] = createSlider;
			nameToFunc["QCheckBox"] = createCheckBox;
			nameToFunc["QSpinBox"] = createSpinBox;
		}

		QString name = typeid(T).name();
		if (!nameToFunc.contains(name.remove("class ")))
			return nullptr;

		QWidget* pWidget = nameToFunc[name](property, parent);
		if (!pWidget)
			return nullptr;

		pWidget->setObjectName(property.getObjectname());
		pWidget->setEnabled(property.getEnabledState());
		pWidget->setStyleSheet(property.getStylesheet());
		pWidget->setToolTip(property.getToolTips());

		return qobject_cast<T*>(pWidget);
	}
};

