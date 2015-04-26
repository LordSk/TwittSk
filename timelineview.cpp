#include <QDir>
#include <QDesktopServices>

#include "timeline.h"
#include "timelineview.h"

TimelineView::TimelineView(Timeline *timeline, QSize size, QWidget *parent):
    QWebView(parent),
    _timeline(timeline),
    _newTweetsCount(0),
    _firstFetch(true)
{
    setMinimumSize(size);

    page()->setLinkDelegationPolicy(QWebPage::LinkDelegationPolicy::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));

    connect(_timeline, SIGNAL(topFetched(int)), this, SLOT(topFetched(int)));
}

TimelineView::~TimelineView()
{

}

void TimelineView::markAsRead()
{
    _newTweetsCount = 0;
}

void TimelineView::updateHTML()
{
    QString html = "";
    QTextStream out(&html, QIODevice::WriteOnly);

    out
    << "<!DOCTYPE html>"
    << "<html>"
    << "<head>"
    << "<meta charset=\"utf-8\">"
    << "<link rel=\"stylesheet\" type=\"text/css\" href=\"" << "file:///" + QDir::currentPath() + "/twittsk.css" << "\">"
    << "</head>"
    << "<body>";

    // open newTweets block
    if(_newTweetsCount > 0)
        out << "<div class=newTweets>";

    int ntc = _newTweetsCount;
    const auto& tweets = _timeline->getTweets();

    for(const auto& pair : tweets) {
        // close newTweets block
        if(ntc-- == 0 && _newTweetsCount > 0)
           out << "</div>";

        out << pair.second.toHTML();
    }

    out << "</body></html>";


    setHtml(html);
}

void TimelineView::linkClicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

void TimelineView::topFetched(int newTweetsCount)
{
    if(newTweetsCount == 0) // nothing new, don't update the view
        return;

#ifdef NDEBUG
    if(_firstFetch) { // first fetch
        _firstFetch = false;
        _newTweetsCount = 0;
    }
    else {
        _newTweetsCount += newTweetsCount;
    }
#else
    _newTweetsCount = 5;
#endif

    updateHTML();

    emit newTweets(_newTweetsCount);
}

