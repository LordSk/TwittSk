#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QWebView>

class TimelineView : protected QWebView
{
    Q_OBJECT

public:
    explicit TimelineView(class Timeline* timeline, QSize size, QWidget* parent = nullptr);
    ~TimelineView();

private:
    class Timeline* _timeline;

private slots:
    void linkClicked(const QUrl& url);
    void homeTlTop(int newTweetsCount);
};

#endif // TIMELINEVIEW_H
