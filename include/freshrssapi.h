#ifndef NEWSBOAT_FRESHRSSAPI_H_
#define NEWSBOAT_FRESHRSSAPI_H_

#include <libxml/tree.h>

#include "3rd-party/json.hpp"

#include "cache.h"
#include "remoteapi.h"
#include "rss/feed.h"
#include "utf8string.h"
#include "utils.h"

using HTTPMethod = newsboat::utils::HTTPMethod;

namespace newsboat {

class FreshRssApi : public RemoteApi {
public:
	explicit FreshRssApi(ConfigContainer* c);
	~FreshRssApi() override = default;
	bool authenticate() override;
	std::vector<TaggedFeedUrl> get_subscribed_urls() override;
	void add_custom_headers(curl_slist** custom_headers) override;
	bool mark_all_read(const std::string& feedurl) override;
	bool mark_article_read(const std::string& guid, bool read) override;
	bool update_article_flags(const std::string& oldflags,
		const std::string& newflags,
		const std::string& guid) override;
	rsspp::Feed fetch_feed(const std::string& id, CURL* cached_handle);

private:
	std::vector<std::string> get_tags(xmlNode* node);
	std::string get_new_token();
	bool refresh_token();
	std::string retrieve_auth();
	std::string post_content(const std::string& url,
		const std::string& postdata);
	bool star_article(const std::string& guid, bool star);
	bool share_article(const std::string& guid, bool share);
	bool mark_article_read_with_token(const std::string& guid,
		bool read,
		const std::string& token);
	Utf8String auth;
	Utf8String auth_header;
	bool token_expired;
	Utf8String token;
};

} // namespace newsboat

#endif /* NEWSBOAT_FRESHRSSAPI_H_ */
