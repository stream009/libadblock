#include "mock_context.hpp"

#include "adblock.hpp"
#include "context.hpp"
#include "type.hpp"

#include <iostream>

#include <boost/range/algorithm.hpp>
#include <boost/timer/timer.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(AdBlock, Statistics)
{
    std::locale locale { "" };
    std::locale::global(locale);
    std::cout.imbue(locale);

    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addSubscription(path);

    adBlock.statistics(std::cout);
}

TEST(AdBlock, DISABLED_EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addSubscription(path);

    {
        boost::timer::auto_cpu_timer t;
        const MockContext context;
#if 0
        EXPECT_TRUE(
                adBlock.shouldBlock("http://a.kickass.to/sc-b98b537.js"_u, context));
#endif
        for (auto i = 0u; i < 1000; ++i) {
            adBlock.shouldBlock("http://a.kickass.to/sc-b98b537.js"_u, context);
        }
    }
    {
        boost::timer::auto_cpu_timer t;
        const MockContext context;
#if 0
        EXPECT_FALSE(
                adBlock.shouldBlock("http://barhbarhbarh.com"_u, context));
#endif
        for (auto i = 0u; i < 1000; ++i) {
            adBlock.shouldBlock("http://barhbarhbarh.com"_u, context);
        }
    }
#if 0
    const std::vector<Uri> blockUrls = {
        "http://adrotator.se/?placement=400229&cookie=1&random=0.028944230638444424&domain=uploadrocket.net"_u,
        "http://clkmon.com/adServe/banners?tid=SD1UPLOADROCKET_27179_0&tagid=2"_u,
        "http://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"_u,
        "https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"_u,
        "http://c1.popads.net/pop.js"_u,
        "http://c2.popads.net/pop.js"_u,
        "http://tempest.services.disqus.com/listPromoted?callback=jQuery111205239674453623593_1430028539529&thread=3709066079&count=8&top_placement=sponsored_links"_u,

        // third-party
        //"http://c.actiondesk.com/?s=58727&p=26&pp=7"_u,
        //"http://cdn.mobicow.com/deliver/p/4273/2985/1/intv2/900"_u,
        //"http://cdn.directrev.com/js/gp.min.js?s=S0003121"_u,
        //"http://login.dotomi.com/ucm/UCMController?dtm_com=28&dtm_fid=105&dtm_cid=2932&dtm_cmagic=c42942&dtm_format=5&cli_promo_id=1"_u,
        //"http://p.nexac.com/e/sr/a-2079/s-4312/s-4312.xgi"_u,
        //"http://creative.wwwpromoter.com/133.js?d=300x250"_u,
        //"http://creative.wwwpromoter.com/133.js?d=728x90"_u,

        //"http://www.toptrailers.net/"_u, //||toptrailers.net^$popup,domain=kingfiles.net|uploadrocket.net
    };
    for (const auto &uri: blockUrls) {
        const MockContext context;
        ASSERT_TRUE(uri.is_valid()) << uri.string();
        ASSERT_TRUE(adBlock.shouldBlock(uri, context)) << uri.string();
    }

    const std::vector<Uri> passUrls = {
        "http://185.63.252.104/files/6/jn7oeq3w598rpi/t.s17e06.u413923.Rapidmoviez.com.rar"_u,
        "http://a.disquscdn.com/embed.js"_u,
        "http://a0e085f0b36bc8e6cddd1c52e2ef8919c.profile.icn51.cloudfront.net/test.png"_u,
        "http://a203d537b76ff788fef59373e2edcf46e.profile.arn1.cloudfront.net/test.png"_u,
        "http://a528d6000aeb414581fb182a56c81eb24.profile.syd1.cloudfront.net/test.png"_u,
        "http://a588a0919fad7ec999744c465785c6f24.profile.fra6.cloudfront.net/test.png"_u,
        "http://a5cee7.se/?placement=400229&async"_u,
        "http://a88154ee7cd8e8722e8093f3002df50dd.profile.iad12.cloudfront.net/test.png"_u,
        "http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.js"_u,
        "http://ajax.googleapis.com/ajax/libs/webfont/1/webfont.js"_u,
        "http://api.recaptcha.net/challenge?k=6Lfm3d8SAAAAAO8YhYrgbsVwL5HUxy-T8R9DlklV&error="_u,
        "http://api.solvemedia.com/media/info-whV2.gif"_u,
        "http://api.solvemedia.com/media/note-whV2.gif"_u,
        "http://api.solvemedia.com/media/reload-whV2.gif"_u,
        "http://api.solvemedia.com/media/solve-whV2.png"_u,
        "http://api.solvemedia.com/media/text-whV2.gif"_u,
        //"http://api.solvemedia.com/papi/_challenge.js?k=mC2C7c.3-sHSuvEpXYQrUJ-TQy3PH2ET;f=_ACPuzzleUtil.callbacks[0];l=en;t=img;s=standard;c=js,h5c,h5ct,svg,h5v,v/h264,v/ogg,v/3gpp,v/webm,h5a,a/mp3,a/ogg,touch,ua/safari,expand,swf11,swf11.2,swf,fwv/MUkZhQ.flfl44,jslib/jquery,htmlplus;am=duVhDrvhZdZHrHvBu-Fl1g;ca=script;ts=1430027744;ct=1430027946;th=white;r=0.44929215707816184"_u,
        "http://api.solvemedia.com/papi/_puzzle.js"_u,
        "http://api.solvemedia.com/papi/challenge.script?k=mC2C7c.3-sHSuvEpXYQrUJ-TQy3PH2ET"_u,
        "http://api.solvemedia.com/papi/media?c=2@mC2C7c.3-sHSuvEpXYQrUJ-TQy3PH2ET@UQnO0teIOw4ENU8bgrS4MX9APTC4lJjoU2fCFhN6OdlKIyOGCOsz4bBMR5VQQ97iZBL2HJ1LDwGuwzhseihxPU0ZOywW7UWtv5LVLoWI.Q5.TieH3P0GY-pWyS4k9a01m0msrVhcsWOcMilVLdlvmE9IkdarHNTGHlsnKrtKu7PfX8UVdrkwOTvHzb.ZIPAYNDz5AvsMM7mhI-NvQjOTScpEr0J0R2mdHZyDYkgzU8doDAHN8le0PuyPrjMDgYOKY-DWUz0TADUSIt49vPvS3lV1JeUIuFISJkhFXIK0uoA;w=300;h=150;fg=000000;bg=f8f8f8"_u,
        //"http://b.scorecardresearch.com/b?c1=8&c2=14651931&c3=1000000000000000001&ns__t=1430028557207&ns_c=UTF-8&c8=Download r3s5awbpbyor from attiadonapt&c7=http://uploadrocket.net/r3s5awbpbyor&c9=http://uploadrocket.net/r3s5awbpbyor"_u,
        "http://b.scorecardresearch.com/beacon.js"_u,
        "http://cloudfront-labs.amazonaws.com/x.png"_u,
        "http://cloudstatic.net/td/pic/pn_inbox.gif"_u,
        "http://cloudstatic.net/td/pic/pn_sentbox.gif"_u,
        "http://disqus.com/api/3.0/discovery/listRelated.json?thread=3709066079&limit=8&api_key=E8Uh5l5fHZ6gD8U3KycjAIAk46f68Zw7C6eW8WSjZvCLXebZ7p0r1yrYDrLilk2F"_u,
        //"http://disqus.com/embed/comments/?base=default&version=f46e9ff9693ef6bae3d43ee13444e437&f=tvrls&t_u=http://tv-release.net/428187/The Last Days Of S01E03 Charles I PDTV x264-C4TV&t_d=The Last Days Of S01E03 Charles I PDTV x264-C4TV | TV-RELEASE.NET&t_t=The Last Days Of S01E03 Charles I PDTV x264-C4TV | TV-RELEASE.NET&s_o=default#2"_u,
        //"http://disqus.com/embed/comments/?base=default&version=f46e9ff9693ef6bae3d43ee13444e437&f=tvrls&t_u=http://tv-release.net/428228/Cyber Dreaming PDTV x264-CBFM&t_d=Cyber Dreaming PDTV x264-CBFM | TV-RELEASE.NET&t_t=Cyber Dreaming PDTV x264-CBFM | TV-RELEASE.NET&s_o=default#2"_u,
        //"http://disqus.com/embed/comments/?base=default&version=f46e9ff9693ef6bae3d43ee13444e437&f=tvrls&t_u=http://tv-release.net/428230/About Tonight S02E08 Nina Oyama PDTV x264-CBFM&t_d=About Tonight S02E08 Nina Oyama PDTV x264-CBFM | TV-RELEASE.NET&t_t=About Tonight S02E08 Nina Oyama PDTV x264-CBFM | TV-RELEASE.NET&s_o=default#2"_u,
        //"http://disqus.com/embed/comments/?base=default&version=f46e9ff9693ef6bae3d43ee13444e437&f=tvrls&t_u=http://tv-release.net/428254/Give A Pet A Home S01E02 HDTV XviD-AFG&t_d=Give A Pet A Home S01E02 HDTV XviD-AFG | TV-RELEASE.NET&t_t=Give A Pet A Home S01E02 HDTV XviD-AFG | TV-RELEASE.NET&s_o=default#2"_u,
        //"http://disqus.com/embed/comments/?base=default&version=f46e9ff9693ef6bae3d43ee13444e437&f=tvrls&t_u=http://tv-release.net/428755/Outlander S01E12 HDTV XviD-AFG&t_d=Outlander S01E12 HDTV XviD-AFG | TV-RELEASE.NET&t_t=Outlander S01E12 HDTV XviD-AFG | TV-RELEASE.NET&s_o=default#2"_u,
        "http://disqus.com/next/config.js"_u,
        "http://g-ecx.images-amazon.com/images/G/01/imdb/plugins/rating/css/imdbRating.min.css"_u,
        "http://lookpic.com/O/i2/529/tcbpUt0q.jpeg"_u,
        "http://p.media-imdb.com/static-content/documents/v1/title/tt0213373/ratings?jsonp=imdb.rating.run:imdb.api.title.ratings/data.json?u=ur24104450&s=p3"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/bl.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/bm.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/br.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/closebox.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/ml.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/mr.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/tl.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/tm.png"_u,
        "http://rapidmoviez.com/files/img/fancyzoom/tr.png"_u,
        "http://rapidmoviez.com/files/img/favicon.ico"_u,
        "http://rapidmoviez.com/files/img/icons/email.png"_u,
        "http://rapidmoviez.com/files/img/icons/imdb_37x18.png"_u,
        "http://rapidmoviez.com/files/img/icons/rss.png"_u,
        "http://rapidmoviez.com/files/js/zeroclipboard/ZeroClipboard.swf"_u,
        "http://rapidmoviez.com/releases/image/49x70/2011-09/pokemon.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-01/mr.-d.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-02/diners--drive-ins-and-dives.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-04/cops.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-08/childrens-hospital.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-09/criminal-minds.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2012-11/adventure-time-with-finn---jake.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-01/the-americans.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-04/teen-titans-go.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-06/dog-with-a-blog.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-06/hot-in-cleveland.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-06/mickey-mouse.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-07/axe-cop.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-07/the-soul-man.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-09/mom.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-10/instant-mom.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-10/reign.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-10/supernatural.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2013-11/steven-universe.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-01/casualty.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-02/clarence.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-02/inside-no.-9.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-03/w1a.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-04/baby-steps.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-05/louie.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-06/fairy-tail-2014.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-09/black-ish.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-09/the-blacklist.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/bones.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/grey-s-anatomy.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/marry-me.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/nashville.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/scandal.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-10/the-vampire-diaries.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2014-11/sonic-boom.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-01/backstrom.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-01/bella-and-the-bulldogs.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-01/top-gear.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/allegiance.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/dig.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/elementary.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/jupiter-ascending.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/the-odd-couple.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-02/vikings.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-03/american-crime.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-03/big-time-in-hollywood--fl.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/absolution.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/banished.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/batman-unlimited--animal-instincts.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/furious-7.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/just-eat-it--a-food-waste-story.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/olympus.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/the-red-road.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/thunderbirds-are-go.jpg"_u,
        "http://rapidmoviez.com/releases/image/49x70/2015-04/wild-in-blue.jpg"_u,
        "http://rapidrar.com/captchas/munln4zt6mtpmgtqntikgddw7g6eh5h6kt7pigy.jpg"_u,
        "http://rapidrar.com/favicon.ico"_u,
        "http://rapidrar.com/font/HelveticaNeueThn.ttf"_u,
        "http://rapidrar.com/images/flags.png"_u,
        "http://rapidrar.com/images/rrlogo.png"_u,
        "http://rapidrar.com/jquery-1.9.1.min.js"_u,
        "http://rapidrar.com/jquery.cookie.js"_u,
        "http://rapidrar.com/js/rapidrar.js"_u,
        "http://rapidrar.com/main.css"_u,
        "http://rapidrar.com/o1kb7rmmvej7"_u,
        "http://rapidrar.com/tabber.js"_u,
        "http://rapidrar.com/xupload.js?"_u,
        "http://realtime.services.disqus.com/api/2/thread/3709066079?bust=1"_u,
        "http://realtime.services.disqus.com/api/2/thread/3709225212?bust=1"_u,
        "http://realtime.services.disqus.com/api/2/thread/3709240014?bust=1"_u,
        "http://realtime.services.disqus.com/api/2/thread/3709756006?bust=1"_u,
        "http://realtime.services.disqus.com/api/2/thread/3710045201?bust=3"_u,
        "http://realtime.services.disqus.com/api/2/thread/3713046742?bust=1"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=&variant=&product=embed&thread=3709066079&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428187/The+Last+Days+Of+S01E03+Charles+I+PDTV+x264-C4TV&page_referrer=http://tv-release.net/?s=The%20Last%20Days%20Of%20S01E03&cat=&major_version=midway&internal_organic=4&external_organic=0&promoted=0&display=true&placement=top&area=discovery&thread_id=3709066079&bin=&promoted_ids=[]&object_type=advertisement&object_id=[]&advertisement_id=[]&ad_product_name=sponsored_links&ad_product_layout=thumbnails&event=activity.load_advertisement&verb=load&imp=3tn796d3ji7&prev_imp=44oq4r79h45&section=default"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=&variant=&product=embed&thread=3709225212&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428230/About+Tonight+S02E08+Nina+Oyama+PDTV+x264-CBFM&page_referrer=http://tv-release.net/?s=About%20Tonight%20S02E08&cat=&major_version=midway&internal_organic=4&external_organic=0&promoted=0&display=true&placement=top&area=discovery&thread_id=3709225212&bin=&promoted_ids=[]&object_type=advertisement&object_id=[]&advertisement_id=[]&ad_product_name=sponsored_links&ad_product_layout=thumbnails&event=activity.load_advertisement&verb=load&imp=3phs2el5ful&prev_imp=3re52df1ou4&section=default"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=&variant=&product=embed&thread=3709240014&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428228/Cyber+Dreaming+PDTV+x264-CBFM&page_referrer=http://tv-release.net/?s=Cyber%20Dreaming&cat=&major_version=midway&internal_organic=4&external_organic=0&promoted=0&display=true&placement=top&area=discovery&thread_id=3709240014&bin=&promoted_ids=[]&object_type=advertisement&object_id=[]&advertisement_id=[]&ad_product_name=sponsored_links&ad_product_layout=thumbnails&event=activity.load_advertisement&verb=load&imp=3nvlpr318f6&prev_imp=3phs2el5ful&section=default"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=&variant=&product=embed&thread=3709756006&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428254/Give+A+Pet+A+Home+S01E02+HDTV+XviD-AFG&page_referrer=http://tv-release.net/?s=Give%20A%20Pet%20A%20Home%20S01E02&cat=&major_version=midway&internal_organic=4&external_organic=0&promoted=0&display=true&placement=top&area=discovery&thread_id=3709756006&bin=&promoted_ids=[]&object_type=advertisement&object_id=[]&advertisement_id=[]&ad_product_name=sponsored_links&ad_product_layout=thumbnails&event=activity.load_advertisement&verb=load&imp=3re52df1ou4&prev_imp=3tn796d3ji7&section=default"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=&variant=&product=embed&thread=3713046742&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428755/Outlander+S01E12+HDTV+XviD-AFG&page_referrer=http://tv-release.net/?s=Outlander%20S01E12%20HDTV%20XviD-AFG&cat=&major_version=midway&internal_organic=4&external_organic=0&promoted=0&display=true&placement=top&area=discovery&thread_id=3713046742&bin=&promoted_ids=[]&object_type=advertisement&object_id=[]&advertisement_id=[]&ad_product_name=sponsored_links&ad_product_layout=thumbnails&event=activity.load_advertisement&verb=load&imp=3mai2o8taeq&prev_imp=3nvlpr318f6&section=default"_u,
        "http://referrer.disqus.com/juggler/event.js?experiment=default&variant=control&product=embed&thread=3709756006&forum=tvrls&forum_id=3356071&zone=thread&page_url=http://tv-release.net/428254/Give+A+Pet+A+Home+S01E02+HDTV+XviD-AFG&page_referrer=http://tv-release.net/?s=Give%20A%20Pet%20A%20Home%20S01E02&cat=&verb=view&object_type=product&object_id=embed&event=activity&imp=3re52df1ou4&prev_imp=3tn796d3ji7&section=default&area=n/a"_u,
        "http://referrer.disqus.com/juggler/event.js?thread_slug=about_tonight_s02e08_nina_oyama_pdtv_x264_cbfm_tv_releasenet&user_type=anon&referrer=http://tv-release.net/428230/About%20Tonight%20S02E08%20Nina%20Oyama%20PDTV%20x264-CBFM&theme=next&dnt=0&tracking_enabled=1&organic_enabled=true&promoted_enabled=true&max_enabled=true&thumbnails_enabled=true&event=init_embed&thread=3709225212&forum=tvrls&forum_id=3356071&imp=3phs2el5ful&prev_imp=3re52df1ou4"_u,
        "http://referrer.disqus.com/juggler/event.js?thread_slug=cyber_dreaming_pdtv_x264_cbfm_tv_releasenet&user_type=anon&referrer=http://tv-release.net/428228/Cyber%20Dreaming%20PDTV%20x264-CBFM&theme=next&dnt=0&tracking_enabled=1&organic_enabled=true&promoted_enabled=true&max_enabled=true&thumbnails_enabled=true&event=init_embed&thread=3709240014&forum=tvrls&forum_id=3356071&imp=3nvlpr318f6&prev_imp=3phs2el5ful"_u,
        "http://referrer.disqus.com/juggler/event.js?thread_slug=give_a_pet_a_home_s01e02_hdtv_xvid_afg_tv_releasenet&user_type=anon&referrer=http://tv-release.net/428254/Give%20A%20Pet%20A%20Home%20S01E02%20HDTV%20XviD-AFG&theme=next&dnt=0&tracking_enabled=1&organic_enabled=true&promoted_enabled=true&max_enabled=true&thumbnails_enabled=true&event=init_embed&thread=3709756006&forum=tvrls&forum_id=3356071&imp=3re52df1ou4&prev_imp=3tn796d3ji7"_u,
        "http://referrer.disqus.com/juggler/event.js?thread_slug=outlander_s01e12_hdtv_xvid_afg_tv_releasenet&user_type=anon&referrer=http://tv-release.net/428755/Outlander%20S01E12%20HDTV%20XviD-AFG&theme=next&dnt=0&tracking_enabled=1&organic_enabled=true&promoted_enabled=true&max_enabled=true&thumbnails_enabled=true&event=init_embed&thread=3713046742&forum=tvrls&forum_id=3356071&imp=3mai2o8taeq&prev_imp=3nvlpr318f6"_u,
        "http://referrer.disqus.com/juggler/event.js?thread_slug=the_last_days_of_s01e03_charles_i_pdtv_x264_c4tv_tv_releasenet&user_type=anon&referrer=http://tv-release.net/428187/The%20Last%20Days%20Of%20S01E03%20Charles%20I%20PDTV%20x264-C4TV&theme=next&dnt=0&tracking_enabled=1&organic_enabled=true&promoted_enabled=true&max_enabled=true&thumbnails_enabled=true&event=init_embed&thread=3709066079&forum=tvrls&forum_id=3356071&imp=3tn796d3ji7&prev_imp=44oq4r79h45"_u,
        "http://s10.histats.com/js15_as.js"_u,
        "http://s4.histats.com/stats/0.php?1174192&@f16&@g1&@h1&@i20&@j1430028504378&@k235534747&@l112&@mDownload s17e06 u413923 Rapidmoviez com rar&@n0&@ohttp://rapidmoviez.com/release/tonight-s17e06-spotlight-nigel-farage-hdtv-x264-c4tv&@q0&@r0&@s0&@ten-US&@u1280&@vhttp://rapidrar.com/o1kb7rmmvej7&@w"_u,
        "http://s4.histats.com/stats/0.php?1606961&@f16&@g0&@h2&@i114&@j1430028469460&@k13455&@l1050&@mRapidMoviez - [RR/UL/180U] Tonight S17E06 Spotlight Nigel Farage HDTV x264-C4TV (170MB)&@n0&@ohttp://rapidmoviez.com/search/Tonight%20S17E06%20Spotlight%20Nigel%20Farage%20HDTV%20x264-C4TV/releases&@q0&@r0&@s0&@ten-US&@u1280&@vhttp://rapidmoviez.com/release/tonight-s17e06-spotlight-nigel-farage-hdtv-x264-c4tv&@w"_u,
        "http://static.torrentday.me/td/SD-x264.png"_u,
        "http://static.torrentday.me/td/TV-X264.png"_u,
        "http://static.torrentday.me/td/pic/love.png"_u,
        "http://static.torrentday.me/td/pic/on.png"_u,
        "http://static.torrentday.me/td/tor-header.png"_u,
        "http://static.torrentday.me/td/tv-xvidv.png"_u,
        "http://static.torrentday.me/td/txt-header.png"_u,
        "http://tv-release.net/?s=About Tonight S02E08&cat="_u,
        "http://tv-release.net/?s=Cyber Dreaming&cat="_u,
        "http://tv-release.net/?s=Give A Pet A Home S01E02&cat="_u,
        "http://tv-release.net/?s=Outlander S01E12 HDTV XviD-AFG&cat="_u,
        "http://tv-release.net/?s=The Last Days Of S01E03&cat="_u,
        "http://tvrls.disqus.com/embed.js"_u,
        "http://ultraimg.com/images/25e7f5.jpg"_u,
        "http://uploadrocket.net/favicon.ico"_u,
        "http://uploadrocket.net/fq8ekib10iak"_u,
        "http://uploadrocket.net/images/PaymentProcessors/bigshareshop.jpg"_u,
        "http://uploadrocket.net/images/PaymentProcessors/keyinstant.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_all.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_amex.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_bitcoins.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_call2pay.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_cashu.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_direct.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_egopay.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_giropay.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_google_wallet.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_jcb.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_liqpay.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_mastercard.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_okpay.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_paxum.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_payza.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_perfect.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_przelewy24.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_skrill.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_sms.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_sofort.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_visa.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_vorkasse.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_webmoney_wme.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/payments_webmoney_wmz.png"_u,
        "http://uploadrocket.net/images/PaymentProcessors/vipkeys.png"_u,
        "http://uploadrocket.net/images/active.png"_u,
        "http://uploadrocket.net/images/back.png"_u,
        "http://uploadrocket.net/images/button_submit.png"_u,
        "http://uploadrocket.net/images/feature.png"_u,
        "http://uploadrocket.net/images/feature_table.png"_u,
        "http://uploadrocket.net/images/flags.png"_u,
        "http://uploadrocket.net/images/free.png"_u,
        "http://uploadrocket.net/images/login_back.png"_u,
        "http://uploadrocket.net/images/nop.png"_u,
        "http://uploadrocket.net/images/password_back.png"_u,
        "http://uploadrocket.net/images/premium.png"_u,
        "http://uploadrocket.net/images/register.png"_u,
        "http://uploadrocket.net/images/rocketlogo.png"_u,
        "http://uploadrocket.net/images/starmark.png"_u,
        "http://uploadrocket.net/images/yep.png"_u,
        "http://uploadrocket.net/img/spriteBanner.png"_u,
        "http://uploadrocket.net/jquery-1.4.2.min.js"_u,
        "http://uploadrocket.net/oc5njl4ybbub"_u,
        "http://uploadrocket.net/r3s5awbpbyor"_u,
        "http://uploadrocket.net/rocket.css"_u,
        "http://uploadrocket.net/tabber.js"_u,
        "http://uploadrocket.net/xupload.js?"_u,
        "http://uploadrocket.net/zupsmubxqxow"_u,
        "http://uploadrocket.net/zzwqew9bahji"_u,
        "http://www.google.com/js/th/3gvCt7Rmf569tTWeW1FogYZtzXOpsMxCRNo2hV3FIBY.js"_u,
        "http://www.google.com/recaptcha/api/challenge?k=6Lfm3d8SAAAAAO8YhYrgbsVwL5HUxy-T8R9DlklV&error="_u,
        "http://www.google.com/recaptcha/api/image?c=03AHJ_Vus5qAdkmxnxcEFM_hj__NrXECgmbtLQ0q5azX3Oebh1NSC2jlbYHAuwy81GUhGa_kWoXgWG-Qsmz4fyiuvi3jEorRL8YFqEXvrwi3ck9eJQ-OjWgJA5O7dbjQoVtODBNnj2zTuiG1JYVbJISbEArTv3LQqOsx5tNVvXtCzmPcA5b_akadkp2WPCQJffDum7wxvT4I0xCeG1E_vcAs4grFyi3m0E8NDIikpzHt9_FXSAu6dFhFmTjEHk-iUiWzSm-JjCJkq8v6qM8wO6KDLWoa9K8w7n1w&th=,_g-wV0tebg8yvoC28dLgoOuc9SbwAAAB2aAAAAAN2AC5e7A7BhKWucJwiGyKfZa3M1MEpyLMCPzqMAS75xmEkYxqKtUZnQXpy0vOJ0vYMTrJBXG9XjWmGFuSyLKQxI3pIOGco12WNTO_lpYQi4bVLHbuM-dAx_lkvKdAZJ6ex9D8P4RnFMNJwLUc3CemLOUJAVXvBcquatCTt6-5gXUW2XgQVX-TNGtUlDc1ZucaZP3enCUFaR1JqdnQ8MxaZZwpEeFuY4GpTKM89Oij_uKvuF8rfiCpGXfCmvI"_u,
        "http://www.google.com/recaptcha/api/img/white/audio.gif"_u,
        "http://www.google.com/recaptcha/api/img/white/help.gif"_u,
        "http://www.google.com/recaptcha/api/img/white/refresh.gif"_u,
        "http://www.google.com/recaptcha/api/img/white/text.gif"_u,
        "http://www.google.com/recaptcha/api/js/recaptcha.js"_u,
        "http://www.google.com/recaptcha/api/reload?c=03AHJ_VuvHViCkFHu4JdA1y6J69cir_S1P6f25rHS-Ev8M-sOI9ZLPl6OxgLnrTXYYiRVTDKAcG9yIT8f5duSIyfMualRCvXbIHPO0KB-5zKglsI5vXZdXnMY1Xpy-Zj8iTJgetnI1RYtAow5eM6c_GphRzC8fNpqDXHcY3V5nZLwwCmRo1CuLWz-qyF3NrKEn-l1YU8j2O6LQioVSkAvOSax9qfUa5l4LRBTuSifhnoZTSTr77rRzmySU5G_aXbk3qAdQGfwb56FiDzXBCkq7gbU0-6CAH6syzg&k=6Lfm3d8SAAAAAO8YhYrgbsVwL5HUxy-T8R9DlklV&reason=i&type=image&lang=en&th=,BfSwV0tebg8yvoC28dLgoOuc9SbwAAAB2aAAAAA-2AC5e7A7BhKWucJwiGyKfZa3M1MEpyLMCPzqMAS75xmEkYxqKtUZnQXpy0vOJ0vYMTrJBXG9XjWmGFuSyLKQxI3pIOGco12WNTO_lpYQi4bVLHbuM-dAx_lkvKdAZJ6ex9D8P4RnFMNJwLUc3CemLOUJAVXvBcquatCTt6-5gXUW2XgQVX-TNGtUlDc1ZucaZP3enCUFaR1JqdnQ8MxaZZwpEeFuY4GpTKM89Oij_uKqEl8rfi48Uk5vszs"_u,
        "http://www.uploadrocket.net/images/cart.png"_u,
        "http://www.uploadrocket.net/images/dl_left.png"_u,
        "http://www.uploadrocket.net/images/dl_left_green.png"_u,
        "http://www.uploadrocket.net/images/dl_right.png"_u,
        "http://www.uploadrocket.net/images/dl_right_green.png"_u,
        "https://cloudstatic.net/pic/backlink.gif"_u,
        "https://cloudstatic.net/td/CSS/globeCSS.css"_u,
        "https://cloudstatic.net/td/CSS/torrentday-pagerTW.css"_u,
        "https://cloudstatic.net/td/b/GoT5.jpg"_u,
        "https://cloudstatic.net/td/du/DU-NEW.png"_u,
        "https://cloudstatic.net/td/js/VPbox.js"_u,
        "https://cloudstatic.net/td/js/VPrsLzy.js"_u,
        "https://cloudstatic.net/td/js/jqv...js"_u,
        "https://cloudstatic.net/td/pic/arrowdown.gif"_u,
        "https://cloudstatic.net/td/pic/arrowup.gif"_u,
        "https://cloudstatic.net/td/pic/email.png"_u,
        "https://cloudstatic.net/td/pic/facebookbottom.png"_u,
        "https://cloudstatic.net/td/pic/tt.png"_u,
        "https://d31qbv1cthcecs.cloudfront.net/atrk.js"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download fq8ekib10iak from attiadonapt&time=1430028786453&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://tv-release.net/428755/Outlander%20S01E12%20HDTV%20XviD-AFG&host_url=http://uploadrocket.net/fq8ekib10iak&random_number=16557907316&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download fq8ekib10iak from attiadonapt&time=1430028792044&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/fq8ekib10iak&host_url=http://uploadrocket.net/fq8ekib10iak&random_number=9368698766&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download oc5njl4ybbub from attiadonapt&time=1430028680950&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://tv-release.net/428230/About%20Tonight%20S02E08%20Nina%20Oyama%20PDTV%20x264-CBFM&host_url=http://uploadrocket.net/oc5njl4ybbub&random_number=9101119653&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download oc5njl4ybbub from attiadonapt&time=1430028686383&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/oc5njl4ybbub&host_url=http://uploadrocket.net/oc5njl4ybbub&random_number=19751322786&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download r3s5awbpbyor from attiadonapt&time=1430028548502&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://tv-release.net/428187/The%20Last%20Days%20Of%20S01E03%20Charles%20I%20PDTV%20x264-C4TV&host_url=http://uploadrocket.net/r3s5awbpbyor&random_number=9011867932&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download r3s5awbpbyor from attiadonapt&time=1430028554025&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/r3s5awbpbyor&host_url=http://uploadrocket.net/r3s5awbpbyor&random_number=6914565886&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download zupsmubxqxow from attiadonapt&time=1430028630748&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://tv-release.net/428254/Give%20A%20Pet%20A%20Home%20S01E02%20HDTV%20XviD-AFG&host_url=http://uploadrocket.net/zupsmubxqxow&random_number=19331488520&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download zupsmubxqxow from attiadonapt&time=1430028637143&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/zupsmubxqxow&host_url=http://uploadrocket.net/zupsmubxqxow&random_number=19563923673&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download zzwqew9bahji from attiadonapt&time=1430028732142&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://tv-release.net/428228/Cyber%20Dreaming%20PDTV%20x264-CBFM&host_url=http://uploadrocket.net/zzwqew9bahji&random_number=13164540269&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=Download zzwqew9bahji from attiadonapt&time=1430028737743&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/zzwqew9bahji&host_url=http://uploadrocket.net/zzwqew9bahji&random_number=6013719430&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=UploadRocket.net - Easy way to share your fileslt&time=1430028564615&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/r3s5awbpbyor&host_url=http://uploadrocket.net/r3s5awbpbyor&random_number=15296373180&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=UploadRocket.net - Easy way to share your fileslt&time=1430028646551&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/zupsmubxqxow&host_url=http://uploadrocket.net/zupsmubxqxow&random_number=15223837839&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=UploadRocket.net - Easy way to share your fileslt&time=1430028697649&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/oc5njl4ybbub&host_url=http://uploadrocket.net/oc5njl4ybbub&random_number=11181280439&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=UploadRocket.net - Easy way to share your fileslt&time=1430028750113&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/zzwqew9bahji&host_url=http://uploadrocket.net/zzwqew9bahji&random_number=15654996800&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://d5nxst8fruw4z.cloudfront.net/atrk.gif?frame_height=706&frame_width=982&iframe=0&title=UploadRocket.net - Easy way to share your fileslt&time=1430028802457&time_zone_offset=-540&screen_params=1280x1024x24&java_enabled=0&cookie_enabled=1&ref_url=http://uploadrocket.net/fq8ekib10iak&host_url=http://uploadrocket.net/fq8ekib10iak&random_number=16876423515&sess_cookie=9bc8a53014cf44eb17979f39808&sess_cookie_flag=0&user_cookie=ab4ce16714ad80857221ab41112&user_cookie_flag=0&dynamic=true&domain=uploadrocket.net&account=K+CPj1aUyS00G2&jsv=20130128&user_lang=en-US"_u,
        "https://glitter.services.disqus.com/urls/?callback=dsqGlitterResponseHandler&forum_shortname=tvrls&thread_id=3709066079"_u,
        "https://glitter.services.disqus.com/urls/?callback=dsqGlitterResponseHandler&forum_shortname=tvrls&thread_id=3709225212"_u,
        "https://glitter.services.disqus.com/urls/?callback=dsqGlitterResponseHandler&forum_shortname=tvrls&thread_id=3709240014"_u,
        "https://glitter.services.disqus.com/urls/?callback=dsqGlitterResponseHandler&forum_shortname=tvrls&thread_id=3709756006"_u,
        "https://glitter.services.disqus.com/urls/?callback=dsqGlitterResponseHandler&forum_shortname=tvrls&thread_id=3713046742"_u,
        "https://links.services.disqus.com/api/click?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428187/The Last Days Of S01E03 Charles I PDTV x264-C4TV&subId=3356071&v=1&libid=i8y22mf30100zpcq000DAm8syyg764hi2&out=http://uploadrocket.net/r3s5awbpbyor&ref=http://tv-release.net/?s=The%20Last%20Days%20Of%20S01E03&cat=&title=The Last Days Of S01E03 Charles I PDTV x264-C4TV | TV-RELEASE.NET&txt=http://uploadrocket.net/r3s5awbpbyor&jsonp=callback"_u,
        "https://links.services.disqus.com/api/click?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428254/Give A Pet A Home S01E02 HDTV XviD-AFG&subId=3356071&v=1&libid=i8y247o00100zpcq000DAkraafxo8n2wy&out=http://uploadrocket.net/zupsmubxqxow&ref=http://tv-release.net/?s=Give%20A%20Pet%20A%20Home%20S01E02&cat=&title=Give A Pet A Home S01E02 HDTV XviD-AFG | TV-RELEASE.NET&txt=http://uploadrocket.net/zupsmubxqxow&jsonp=callback"_u,
        "https://links.services.disqus.com/api/ping?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428187/The Last Days Of S01E03 Charles I PDTV x264-C4TV&subId=3356071&v=1&jsonp=vglnk_jsonp_14300285432920"_u,
        "https://links.services.disqus.com/api/ping?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428228/Cyber Dreaming PDTV x264-CBFM&subId=3356071&v=1&jsonp=vglnk_jsonp_14300287324400"_u,
        "https://links.services.disqus.com/api/ping?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428230/About Tonight S02E08 Nina Oyama PDTV x264-CBFM&subId=3356071&v=1&jsonp=vglnk_jsonp_14300286809070"_u,
        "https://links.services.disqus.com/api/ping?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428254/Give A Pet A Home S01E02 HDTV XviD-AFG&subId=3356071&v=1&jsonp=vglnk_jsonp_14300286175730"_u,
        "https://links.services.disqus.com/api/ping?format=jsonp&key=cfdfcf52dffd0a702a61bad27507376d&loc=http://tv-release.net/428755/Outlander S01E12 HDTV XviD-AFG&subId=3356071&v=1&jsonp=vglnk_jsonp_14300287867280"_u,
        "https://links.services.disqus.com/api/time.gif?nocache=14300285457231&key=cfdfcf52dffd0a702a61bad27507376d&libId=i8y22mf30100zpcq000DAm8syyg764hi2&time=740&type=clk"_u,
        "https://links.services.disqus.com/api/time.gif?nocache=14300286294181&key=cfdfcf52dffd0a702a61bad27507376d&libId=i8y247o00100zpcq000DAkraafxo8n2wy&time=1109&type=clk"_u,
        "https://recon-ecc.aws-protocol-testing.com/index.html"_u,
        "https://www.torrentday.com/browse.php#/browse.php?&c2=1&c7=1&c26=1&search=&cata=yes"_u,
        "https://www.torrentday.com/favicon.ico?"_u,
        "https://www.torrentday.com/images/cellpic1.gif"_u,
        "https://www.torrentday.com/js/cookie.js"_u,
        "https://www.torrentday.com/js/hashbang.js"_u,
        "https://www.torrentday.com/js/v3.js?beta0.5"_u,
        "https://www.torrentday.com/loading.gif"_u,
        "https://www.torrentday.com/pic/back.png"_u,
        "https://www.torrentday.com/pic/comment.png"_u,
        "https://www.torrentday.com/pic/dl.png"_u,
        "https://www.torrentday.com/pic/files.png"_u,
        "https://www.torrentday.com/pic/zoom.png"_u,
    };
    for (const auto &uri: passUrls) {
        const MockContext context;
        ASSERT_TRUE(uri.is_valid()) << uri.string();
        ASSERT_FALSE(adBlock.shouldBlock(uri, context)) << uri.string();
    }
#endif
}
