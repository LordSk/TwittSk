#include <QDir>
#include <QDesktopServices>

#include "timeline.h"
#include "timelineview.h"

TimelineView::TimelineView(Timeline *timeline, QSize size, QWidget *parent):
    QWebView(parent),
    _timeline(timeline)
{
    setMinimumSize(size);

    page()->setLinkDelegationPolicy(QWebPage::LinkDelegationPolicy::DelegateAllLinks);
    connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(linkClicked(const QUrl&)));

    connect(_timeline, SIGNAL(topFetched(int)), this, SLOT(homeTlTop(int)));
}

TimelineView::~TimelineView()
{

}

void TimelineView::linkClicked(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

void TimelineView::homeTlTop(int newTweetsCount)
{
    /*if(_hometlFirstUpdate) {
        _homeTimeline.markAsRead();
        showUnreadIcon(0);
        _hometlFirstUpdate = false;
    }
    else {
        showUnreadIcon(newTweetsCount);
    }

    if(newTweetsCount == 0) // nothing new, don't update the view
        return;*/

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
    if(newTweetsCount > 0)
        out << "<div class=newTweets>";

    int ntc = newTweetsCount;
    const auto& tweets = _timeline->getTweets();

    for(const auto& pair : tweets) {
        // close newTweets block
        if(ntc-- == 0 && newTweetsCount > 0)
           out << "</div>";

        out << pair.second.toHTML();
    }

    out << "</body></html>";


    setHtml(html);
}

