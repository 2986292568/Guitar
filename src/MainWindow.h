#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Git.h"
#include "RepositoryData.h"
#include <memory>
#include <functional>
#include "GitHubAPI.h"
#include "main.h"
#include "TextEditorTheme.h"

namespace Ui {
class MainWindow;
}

class QScrollBar;

class QListWidget;
class QListWidgetItem;
class QTreeWidgetItem;
class QTableWidgetItem;
class AboutDialog;

class MySettings;

class LocalSocketReader;

class CommitList;
class WebContext;

#define PATH_PREFIX "*"


class HunkItem {
public:
	int hunk_number = -1;
	size_t pos, len;
	std::vector<std::string> lines;
};

class MainWindow : public QMainWindow {
	Q_OBJECT
	friend class ImageViewWidget;
	friend class FileDiffSliderWidget;
	friend class FileHistoryWindow;
	friend class FileDiffWidget;
	friend class AboutDialog;
public:
	struct Label {
		enum {
			Head,
			LocalBranch,
			RemoteBranch,
			Tag,
		};
		int kind;
		QString text;
		Label(int kind = LocalBranch)
			: kind(kind)
		{

		}
	};
private:
	struct Private;
	Private *m;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	QPixmap const &digitsPixmap() const;

	QString currentWorkingCopyDir() const;

	static QString makeRepositoryName(QString const &loc);
	const Git::CommitItemList *logs() const;
	QColor color(unsigned int i);
private:
	Ui::MainWindow *ui;

	enum class FilesListType {
		SingleList,
		SideBySide,
	};

	void addDiffItems(const QList<Git::Diff> *diff_list, std::function<void (const QString &, QString, int)> AddItem_);
	void updateFilesList(QString id, bool wait);
	void updateFilesList(const Git::CommitItem &commit, bool wait);
	void updateRepositoriesList();
	QString getBookmarksFilePath() const;
	bool saveRepositoryBookmarks() const;

	void openRepository_(GitPtr g);
	void openRepository(bool validate, bool waitcursor = true);
	void reopenRepository(bool log, std::function<void(GitPtr)> callback);

	void openSelectedRepository();
	bool askAreYouSureYouWantToRun(const QString &title, const QString &command);
	void resetFile(const QStringList &path);
	void revertAllFiles();
	void prepareLogTableWidget();
	QStringList selectedFiles_(QListWidget *listwidget) const;
	QStringList selectedFiles() const;
	void for_each_selected_files(std::function<void (const QString &)> fn);
	bool editFile(const QString &path, const QString &title);
	void updateCommitGraph();
	void showFileList(FilesListType files_list_type);

	bool checkGitCommand();
	bool checkFileCommand();
	void checkUser();

	void clearLog();
	void clearFileList();
	void clearDiffView();
	void clearRepositoryInfo();

	int repositoryIndex_(const QTreeWidgetItem *item) const;
	RepositoryItem const *repositoryItem(const QTreeWidgetItem *item) const;

	bool makeDiff(QString id, QList<Git::Diff> *out);

	void commit(bool amend = false);
	void commit_amend();
	void queryBranches(GitPtr g);
	void updateRemoteInfo();
	QList<Git::Branch> findBranch(const QString &id);
	QList<Git::Tag> findTag(const QString &id);
	int selectedLogIndex() const;
	const Git::CommitItem *selectedCommitItem() const;
	void deleteTags(const Git::CommitItem &commit);
	QTreeWidgetItem *newQTreeWidgetFolderItem(const QString &name);
	void buildRepoTree(const QString &group, QTreeWidgetItem *item, QList<RepositoryItem> *repos);
	void refrectRepositories();

	bool saveByteArrayAs(const QByteArray &ba, const QString &dstpath);
	bool saveFileAs(const QString &srcpath, const QString &dstpath);
	bool saveBlobAs(const QString &id, const QString &dstpath);
	QString selectCommand_(const QString &cmdname, const QStringList &cmdfile, const QStringList &list, QString path, std::function<void (const QString &)> callback);
	QString selectCommand_(const QString &cmdname, const QString &cmdfile, const QStringList &list, QString path, std::function<void(const QString &)> callback);
	void updateDiffView(QListWidgetItem *item);
	void updateDiffView();
	void updateUnstagedFileCurrentItem();
	void updateStagedFileCurrentItem();
//	void addTag();
	QStringList whichCommand_(const QString &cmdfile1, const QString &cmdfile2 = QString());
	QString getObjectID(QListWidgetItem *item);
	QString determinFileType_(const QString &path, bool mime, std::function<void(QString const &cmd, QByteArray *ba)> callback) const;
	Git::Object cat_file_(GitPtr g, const QString &id);
	Git::Object cat_file(const QString &id);
	void updateStatusBarText();
	void setUnknownRepositoryInfo();
	void setWindowTitle_(const Git::User &user);
	void setRepositoryInfo(const QString &reponame, const QString &brname);
	void updateWindowTitle(GitPtr g);
	void logGitVersion();
	static bool git_callback(void *cookie, const char *ptr, int len);
	int indexOfRepository(const QTreeWidgetItem *treeitem) const;
	void removeRepositoryFromBookmark(int index, bool ask);

	enum NamedCommitFlag {
		Branches = 0x0001,
		Tags     = 0x0002,
		Remotes  = 0x0100,
	};
	NamedCommitList namedCommitItems(int flags);

	void deleteBranch(const Git::CommitItem *commit);
	void checkout();
	void clone();
	void deleteBranch();
	Git::CommitItemList retrieveCommitLog(GitPtr g);
	bool runOnRepositoryDir(std::function<void(QString)> callback, const RepositoryItem *repo);
	void openTerminal(const RepositoryItem *repo);
	void openExplorer(const RepositoryItem *repo);
	void pushSetUpstream(const QString &remote, const QString &branch);
	bool pushSetUpstream(bool testonly);
	void clearRepoFilter();
	void appendCharToRepoFilter(ushort c);
	void backspaceRepoFilter();
	void revertCommit();
	int rowFromCommitId(const QString &id);
	void cherrypick(const Git::CommitItem *commit);
	void mergeBranch(const Git::CommitItem *commit);
	void rebaseBranch(const Git::CommitItem *commit);
	void detectGitServerType(GitPtr g);
	void initNetworking();
	void setRemoteOnline(bool f);
	bool isRemoteOnline() const;
	void startTimers();
	void onCloneCompleted(bool success);
	bool fetch(GitPtr g);
	void stopPtyProcess();
	void abortPtyProcess();
	void setNetworkingCommandsEnabled(bool f);
	void execSetUserDialog(const Git::User &global_user, const Git::User &repo_user, const QString &reponame);
	bool execSetGlobalUserDialog();
	void blame(QListWidgetItem *item);
	void blame();
	QListWidgetItem *currentFileItem() const;
	const RepositoryItem *findRegisteredRepository(QString *workdir) const;
	void execAreYouSureYouWantToContinueConnectingDialog();
	void queryRemotes(GitPtr g);
	void internalSetCommand(const QString &path, bool save, const QString &name, QString *out);
	void deleteRemoteBranch(const Git::CommitItem *commit);
	QStringList remoteBranches(const QString &id);
	void rebaseOnto();
	void createRepository(const QString &dir);
	void checkRemoteUpdate();
	void setWatchRemoteInterval(int mins);
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void timerEvent(QTimerEvent *);
	void keyPressEvent(QKeyEvent *event);
	bool event(QEvent *event);
	bool eventFilter(QObject *watched, QEvent *event);
public:

	QString selectGitCommand(bool save);
	QString selectFileCommand(bool save);
	QString selectGpgCommand(bool save);

	int limitLogCount() const;

	void setGitCommand(const QString &path, bool save);
	void setFileCommand(const QString &path, bool save);
	void setGpgCommand(const QString &path, bool save);
	void clearAuthentication();
	void setCurrentRepository(const RepositoryItem &repo, bool clear_authentication);
	QString currentRepositoryName() const;
	const Git::Branch &currentBranch() const;
	bool isThereUncommitedChanges() const;
	QString defaultWorkingDir() const;
	void autoOpenRepository(QString dir);
	void saveRepositoryBookmark(RepositoryItem item);
	void drawDigit(QPainter *pr, int x, int y, int n) const;
	int digitWidth() const;
	int digitHeight() const;
	bool isValidWorkingCopy(const GitPtr &g) const;
	QString tempfileHeader() const;
	void deleteTempFiles();
	QString newTempFilePath();
	bool saveAs(const QString &id, const QString &dstpath);
	QString saveAsTemp(const QString &id);
	QString abbrevCommitID(const Git::CommitItem &commit);
	QString findFileID(GitPtr g, const QString &commit_id, const QString &file);
	QString determinFileType(const QString &path, bool mime);
	QString determinFileType(QByteArray in, bool mime);
	QPixmap getTransparentPixmap();
	QString getCommitIdFromTag(const QString &tag);
	void setStatusBarText(const QString &text);
	void clearStatusBarText();
	QString makeCommitInfoText(int row, QList<Label> *label_list);
	void setLogEnabled(GitPtr g, bool f);
	void addWorkingCopyDir(QString dir, QString name, bool open);
	void addWorkingCopyDir(QString dir, bool open)
	{
		addWorkingCopyDir(dir, QString(), open);
	}
	bool isValidRemoteURL(QString const &url);
	bool testRemoteRepositoryValidity(const QString &url);
	void removeSelectedRepositoryFromBookmark(bool ask);
	void setCurrentLogRow(int row);
	GitPtr git(const QString &dir) const;
	GitPtr git();
	const QList<Label> *label(int row);
	bool isGitHub() const;
	QIcon committerIcon(int row) const;
	void updateCommitTableLater();
	bool isAvatarEnabled() const;
	WebContext *getWebContextPtr();
	bool shown();
	void execFileHistory(QListWidgetItem *item);
	void execFileHistory(const QString &path);
	void execCommitExploreWindow(QWidget *parent, const Git::CommitItem *commit);
	void execCommitPropertyDialog(QWidget *parent, const Git::CommitItem *commit);
	void checkout(QWidget *parent, const Git::CommitItem *commit);
	TextEditorThemePtr themeForTextEditor();
	bool queryCommit(const QString &id, Git::CommitItem *out);
	void jumpToCommit(QString id);
	WebContext *webContext();
	bool execWelcomeWizardDialog();
	void updateFilesList(QString id, QList<Git::Diff> *diff_list, QListWidget *listwidget);
	void execCommitViewWindow(const Git::CommitItem *commit);
	QAction *addMenuActionProperty(QMenu *menu);
	void execFilePropertyDialog(QListWidgetItem *item);
	void execRepositoryPropertyDialog(QString workdir, bool open_repository_menu = false);
	QStringList remotes() const;
	QIcon verifiedIcon(char s) const;
	ApplicationSettings *appsettings();
	const ApplicationSettings *appsettings() const;
	const Git::CommitItem *commitItem(int row) const;
	bool checkExecutable(const QString &path);
	void deleteTags(const QStringList &tagnames);
	QList<Git::Tag> queryTagList();
	bool addTag(const QString &name);
	void updateCurrentFilesList();
	void setRemoteChanged(bool f);
public slots:
	void writeLog(const char *ptr, int len);
	void writeLog(const QString &str);
	void writeLog(QByteArray ba);
private slots:
	void on_action_clone_triggered();
	void on_action_commit_triggered();
	void on_action_edit_git_config_triggered();
	void on_action_edit_gitignore_triggered();
	void on_action_edit_global_gitconfig_triggered();
	void on_action_edit_settings_triggered();
	void on_action_fetch_triggered();
	void on_action_open_existing_working_copy_triggered();
	void on_action_pull_triggered();
	void on_action_push_triggered();
	void on_action_test_triggered();
	void on_action_view_refresh_triggered();
	void on_action_tag_push_all_triggered();

	void on_treeWidget_repos_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_treeWidget_repos_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_tableWidget_log_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

	void on_treeWidget_repos_customContextMenuRequested(const QPoint &pos);
	void on_tableWidget_log_customContextMenuRequested(const QPoint &pos);
	void on_listWidget_files_customContextMenuRequested(const QPoint &pos);
	void on_listWidget_unstaged_customContextMenuRequested(const QPoint &pos);
	void on_listWidget_staged_customContextMenuRequested(const QPoint &pos);

	void on_listWidget_unstaged_currentRowChanged(int currentRow);
	void on_listWidget_staged_currentRowChanged(int currentRow);
	void on_listWidget_files_currentRowChanged(int currentRow);

	void on_toolButton_commit_clicked();
	void on_toolButton_pull_clicked();
	void on_toolButton_push_clicked();
	void on_toolButton_select_all_clicked();
	void on_toolButton_stage_clicked();
	void on_toolButton_unstage_clicked();
	void on_action_about_triggered();
	void on_toolButton_clone_clicked();
	void on_toolButton_fetch_clicked();

	void on_lineEdit_filter_textChanged(const QString &text);
	void on_toolButton_erase_filter_clicked();

	void on_tableWidget_log_itemDoubleClicked(QTableWidgetItem *);
	void on_listWidget_unstaged_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_staged_itemDoubleClicked(QListWidgetItem *item);
	void on_listWidget_files_itemDoubleClicked(QListWidgetItem *item);

	void onRepositoriesTreeDropped();
	void on_action_set_config_user_triggered();

	void on_action_window_log_triggered(bool checked);

	void onLogVisibilityChanged();

	void on_action_repo_jump_triggered();
	void on_action_repo_checkout_triggered();
	void on_action_delete_branch_triggered();

	void on_toolButton_terminal_clicked();
	void on_toolButton_explorer_clicked();

	void on_action_reset_HEAD_1_triggered();

	void on_action_create_a_repository_triggered();

	void onAvatarUpdated();
	void on_radioButton_remote_online_clicked();

	void on_radioButton_remote_offline_clicked();
	void on_verticalScrollBar_log_valueChanged(int);
	void on_horizontalScrollBar_log_valueChanged(int);

	void onPtyProcessCompleted();
	void on_toolButton_stop_process_clicked();

	void on_action_stop_process_triggered();

	void on_action_exit_triggered();

	void on_action_reflog_triggered();


	void on_action_repository_property_triggered();
	void on_action_set_gpg_signing_triggered();
	void on_action_edit_tags_triggered();

    void on_action_push_u_triggered();

	void on_action_delete_remote_branch_triggered();

	void on_action_rebase_onto_triggered();

	void doUpdateButton();
signals:
	void onEscapeKeyPressed();
	void asyncCheckRemoteUpdate();
	void updateButton();
protected:
	void closeEvent(QCloseEvent *event);
protected slots:
	void onLogIdle();
};

#endif // MAINWINDOW_H
