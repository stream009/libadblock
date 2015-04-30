#include "mock_context.hpp"

#include "adblock.hpp"
#include "context.hpp"
#include "type.hpp"

#include <boost/range/algorithm.hpp>

#include <gtest/gtest.h>

using namespace adblock;

namespace bfs = boost::filesystem;
static const bfs::path projectRoot { PROJECT_ROOT };

TEST(AdBlock, EasyList)
{
    const auto &path = projectRoot / "test/data/easylist.txt";
    ASSERT_TRUE(bfs::exists(path)) << path;

    AdBlock adBlock;
    adBlock.addSubscription(path);

    {
        const MockContext context;
        EXPECT_TRUE(
                adBlock.shouldBlock("http://a.kickass.to/sc-b98b537.js"_u, context));
    }
    {
        const MockContext context;
        EXPECT_FALSE(
                adBlock.shouldBlock("http://barhbarhbarh.com"_u, context));
    }

    const std::vector<Uri> blockUrls = {
        "http://adrotator.se/?placement=400229&cookie=1&random=0.028944230638444424&domain=uploadrocket.net"_u,
        "http://clkmon.com/adServe/banners?tid=SD1UPLOADROCKET_27179_0&tagid=2"_u,
        "http://rapidmoviez.com/files/img/icons/facebook.png"_u,
        "http://www.facebook.com/plugins/likebox.php?href=http://www.facebook.com/RapidMoviez&width=282&height=185&colorscheme=light&show_faces=true&border_color=#EEEBE2&stream=false&header=false&appId=105039202921875"_u,
        "http://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"_u,
        "https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js"_u,
        "http://c1.popads.net/pop.js"_u,
        "http://c2.popads.net/pop.js"_u,
        "http://c.actiondesk.com/?s=58727&p=26&pp=7"_u,
        "http://cdn.mobicow.com/deliver/p/4273/2985/1/intv2/900"_u,
        "http://cdn.directrev.com/js/gp.min.js?s=S0003121"_u,
        "http://tempest.services.disqus.com/listPromoted?callback=jQuery111205239674453623593_1430028539529&thread=3709066079&count=8&top_placement=sponsored_links"_u,
        "http://login.dotomi.com/ucm/UCMController?dtm_com=28&dtm_fid=105&dtm_cid=2932&dtm_cmagic=c42942&dtm_format=5&cli_promo_id=1"_u,
        "http://www.google-analytics.com/analytics.js"_u,
        "http://www.google-analytics.com/ga.js"_u,
        "https://ssl.google-analytics.com/ga.js"_u,
        "http://p.nexac.com/e/sr/a-2079/s-4312/s-4312.xgi"_u,
        "http://www.toptrailers.net/"_u,
        "http://creative.wwwpromoter.com/133.js?d=300x250"_u,
        "http://creative.wwwpromoter.com/133.js?d=728x90"_u,
    };
    for (const auto &uri: blockUrls) {
        const MockContext context;
        ASSERT_TRUE(uri.is_valid()) << uri.string();
        ASSERT_TRUE(adBlock.shouldBlock(uri, context)) << uri.string();
    }
}
