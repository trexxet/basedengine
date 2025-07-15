#pragma once

namespace Based::RML {

static const char* debugOverlayRmlDoc = R"(
<rml>
<head>
<style>
	body {
		left: 0px; top: 0px; right: 0px; bottom: 0px;
	}
	p {
		float: left; clear: left;
		text-align: left;
	}
	p:hover {
		background: #444444;
	}
</style>
</head>
<body>
	<p id="based_version"></p>
</body>
</rml>
)";

}
