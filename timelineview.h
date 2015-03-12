#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QWebView>

class TimelineView : public QWebView
{
    Q_OBJECT

public:
    explicit TimelineView(class Timeline* timeline, QSize size, QWidget* parent = nullptr);
    ~TimelineView();

    void markAsRead();

private:
    class Timeline* _timeline;
    int _newTweetsCount;
    bool _firstFetch;

    void updateHTML();

signals:
    void newTweets(int count);

private slots:
    void linkClicked(const QUrl& url);
    void topFetched(int newTweetsCount);
};

#endif // TIMELINEVIEW_H
