#ifndef ONYX_KEYBOARD_H_
#define ONYX_KEYBOARD_H_

#include "onyx/data/sketch_point.h"
#include "keyboard_key.h"
#include "tree_view.h"
#include "im_char_selection.h"
#include "single_shot_timer.h"
#include "keyboard_layout_provider.h"
#include "keyboard_layout.h"
#include "keyboard_direction_dialog.h"

namespace sketch
{
class SketchProxy;
};

namespace handwriting
{
class HandwritingWidget;
};

using namespace sketch;

namespace ui
{

///
/// The main interface to the software keyboard.
///
/// To use this class, create an instance and call attachReceiver() to attach
/// the object that should receive QKeyEvent.
///
class KeyBoard : public QFrame
{
    Q_OBJECT
public:
    explicit KeyBoard(QWidget* parent = 0, Qt::WFlags f = 0);
    virtual ~KeyBoard();
    inline void attachReceiver(QObject *r);
    inline QObject * receiver();

    void resetState();

Q_SIGNALS:
    void shifted(bool);
    void capLocked(bool);

public Q_SLOTS:
    void onButtonClicked(QAbstractButton *button);

private Q_SLOTS:
    void onStrokeStarted();
    void onPointAdded(SketchPoint point);
    void onStrokeAdded(const Points & points);
    void onFinishCharacterTimeOut();
    void onTextSelected(const QString & text, int index);
    void onAutoSelect();
    void onHandwritingFunctionClicked(const QModelIndex & index);
    void onDirectionSelected(KeyboardDirection direction);

protected:
    void init(const QLocale & locale);
    void keyReleaseEvent(QKeyEvent *e);
    void resizeEvent(QResizeEvent *re);
    bool event(QEvent *e);

    void handleCapLockPressed();
    void handleShiftPressed();
    void handleSwitchLanguagePressed();
    void handleHandWriting();

    void updateModifiers();
    void postKeyEvent(QEvent::Type type, uint code);
    QChar getRealChar(uint code);

    void displayAssociatedChars(const QString & current_text);
    bool adjustAssociatedChar(const QString & dst_text, int index);

    void displayDirectionArrows(bool display);

private:
    bool shift_;
    bool lock_;
    bool is_handwriting_;

    KeyboardLayoutProvider keyboard_layout_provider_;
    scoped_ptr<KeyboardLayout> keyboard_layout_;

    Qt::KeyboardModifiers modifiers_;

    QVBoxLayout * ver_layout_;
    QVector< shared_ptr<QHBoxLayout> > hor_layouts_;
    scoped_ptr<QButtonGroup> button_group_;
    QVector< shared_ptr<KeyBoardKey> > buttons_;

    QObject *receiver_;

    scoped_ptr<sketch::SketchProxy> sketch_proxy_;
    scoped_ptr<handwriting::HandwritingWidget> handwriting_widget_;
    scoped_ptr<ui::OnyxTreeView> handwriting_functions_view_;
    scoped_ptr<InputMethodCharSelection> im_char_selection_;
    QStandardItemModel handwriting_functions_model_;

    OnyxSingleShotTimer finish_character_timer_;
    OnyxSingleShotTimer auto_select_timer_;
    QStringList candidates_;
    QString     current_text_;

    scoped_ptr<KeyboardDirectionDialog> up_arrow_;
    scoped_ptr<KeyboardDirectionDialog> down_arrow_;
    scoped_ptr<KeyboardDirectionDialog> left_arrow_;
    scoped_ptr<KeyboardDirectionDialog> right_arrow_;

private:
    friend class KeyboardDirectionDialog;
};

///
/// Set the receiver of QKeyEvent generated by the keyboard.
///
void KeyBoard::attachReceiver(QObject *r)
{
    receiver_ = r;
}

QObject * KeyBoard::receiver()
{
    return receiver_;
}

};   // namespace ui

#endif      // ONYX_KEYBOARD_H_
