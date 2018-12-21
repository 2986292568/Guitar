#ifndef FILEDIFFWIDGET_H
#define FILEDIFFWIDGET_H

#include "texteditor/AbstractCharacterBasedApplication.h"
#include <QDialog>
#include "Git.h"
#include "MainWindow.h"
#include "FileViewWidget.h"
#include "FileDiffSliderWidget.h"

namespace Ui {
class FileDiffWidget;
}

enum class ViewType {
	None,
	Left,
	Right
};

typedef Document::Line TextDiffLine;
typedef QList<Document::Line> TextDiffLineList;

struct ObjectContent {
	QString id;
	QString path;
	QByteArray bytes;
	TextDiffLineList lines;
};
typedef std::shared_ptr<ObjectContent> ObjectContentPtr;

class QTableWidgetItem;

class FileDiffWidget : public QWidget
{
	Q_OBJECT
	friend class BigDiffWindow;
public:
	struct DiffData {
		ObjectContentPtr left;
		ObjectContentPtr right;
		std::vector<std::string> original_lines;
		DiffData()
		{
			clear();
		}
		void clear()
		{
			left = ObjectContentPtr(new ObjectContent());
			right = ObjectContentPtr(new ObjectContent());
			original_lines.clear();
		}
	};

	struct DrawData {
		int v_scroll_pos = 0;
		int h_scroll_pos = 0;
		int char_width = 0;
		int line_height = 0;
		QColor bgcolor_text;
		QColor bgcolor_add;
		QColor bgcolor_del;
		QColor bgcolor_add_dark;
		QColor bgcolor_del_dark;
		QColor bgcolor_gray;
		QWidget *forcus = nullptr;
		DrawData();
	};

	enum ViewStyle {
		None,
		SingleFile,
		LeftOnly,
		RightOnly,
		SideBySideText,
		SideBySideImage,
	};

private:
	Ui::FileDiffWidget *ui;

	struct Private;
	Private *m;

	struct InitParam_ {
		ViewStyle view_style = ViewStyle::None;
		QByteArray bytes_a;
		QByteArray bytes_b;
		Git::Diff diff;
		bool uncommited = false;
		QString workingdir;
	};

	ViewStyle viewstyle() const;

	GitPtr git();
	Git::Object cat_file(GitPtr g, QString const &id);

	int totalTextLines() const;

	void resetScrollBarValue();
	void updateSliderCursor();

	int fileviewHeight() const;

	void setDiffText(const Git::Diff &diff, const TextDiffLineList &left, const TextDiffLineList &right);


	void setLeftOnly(QByteArray const &ba, const Git::Diff &diff);
	void setRightOnly(QByteArray const &ba, const Git::Diff &diff);
	void setSideBySide(QByteArray const &ba, const Git::Diff &diff, bool uncommited, QString const &workingdir);
	void setSideBySide_(QByteArray const &ba_a, QByteArray const &ba_b, QString const &workingdir);

	bool isValidID_(QString const &id);

	FileViewType setupPreviewWidget();

	void makeSideBySideDiffData(const Git::Diff &diff, const std::vector<std::string> &original_lines, TextDiffLineList *left_lines, TextDiffLineList *right_lines);
	void onUpdateSliderBar();
	void refrectScrollBar();
	void setOriginalLines_(QByteArray const &ba);
	QString diffObjects(GitPtr g, QString const &a_id, QString const &b_id);
	MainWindow *mainwindow();
protected:
	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *event);
public:
	explicit FileDiffWidget(QWidget *parent = 0);
	~FileDiffWidget();

	void bind(MainWindow *mw);

	void clearDiffView();

	void setSingleFile(QByteArray const &ba, QString const &id, QString const &path);

	void updateControls();
	void scrollToBottom();

	void updateDiffView(const Git::Diff &info, bool uncommited);
	void updateDiffView(QString id_left, QString id_right, QString const &path = QString());

	void setMaximizeButtonEnabled(bool f);
	void setFocusAcceptable(bool f);
	QPixmap makeDiffPixmap(DiffPane pane, int width, int height);
	void setViewType(FileViewType type);
	void setTextCodec(QTextCodec *codec);
	void setTextCodec(char const *name);
private slots:
	void onVerticalScrollValueChanged(int);
	void onHorizontalScrollValueChanged(int);
	void onDiffWidgetWheelScroll(int lines);
	void onScrollValueChanged2(int value);
	void onDiffWidgetResized();
	void on_toolButton_fullscreen_clicked();

	void scrollTo(int value);
	void onMoved(int cur_row, int cur_col, int scr_row, int scr_col);
	void on_toolButton_menu_clicked();

signals:
	void moveNextItem();
	void movePreviousItem();
	void textcodecChanged();
};

#endif // FILEDIFFWIDGET_H
