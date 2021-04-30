#include "ocnewsurlreader.h"

#include "fileurlreader.h"
#include "logger.h"
#include "remoteapi.h"
#include "utils.h"

namespace newsboat {

OcNewsUrlReader::OcNewsUrlReader(const std::string& url_file, RemoteApi* a)
	: file(Utf8String::from_utf8(url_file))
	, api(a)
{
}

OcNewsUrlReader::~OcNewsUrlReader() {}

nonstd::optional<std::string> OcNewsUrlReader::reload()
{
	urls.clear();
	tags.clear();
	alltags.clear();

	FileUrlReader ur(file.to_utf8());
	const auto error_message = ur.reload();
	if (error_message.has_value()) {
		LOG(Level::DEBUG, "Reloading failed: %s", error_message.value());
		// Ignore errors for now: https://github.com/newsboat/newsboat/issues/1273
	}

	std::vector<std::string>& file_urls(ur.get_urls());
	for (const auto& url : file_urls) {
		if (utils::is_query_url(url)) {
			urls.push_back(url);
		}
	}

	std::vector<TaggedFeedUrl> feedurls = api->get_subscribed_urls();

	for (const auto& url : feedurls) {
		LOG(Level::INFO, "added %s to URL list", url.first);
		urls.push_back(url.first);
		tags[url.first] = url.second;
		for (const auto& tag : url.second) {
			LOG(Level::DEBUG, "%s: added tag %s", url.first, tag);
			alltags.insert(tag);
		}
	}

	return {};
}

std::string OcNewsUrlReader::get_source()
{
	return "ownCloud News";
}

} // namespace newsboat
