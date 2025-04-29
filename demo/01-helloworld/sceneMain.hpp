#pragma once

#include <memory>

#include "Engine.hpp"
#include "Scene.hpp"

#include <RmlUi/Debugger.h>

// For this example, we'll embed the RML UI doc
static const char* document_rml = R"(
<rml>
<head>
<style>
	body {
		font: normal bold 18px "Courier New";
		color: white;
		background: #000000;
		/* Edges must be set explicitely for <body>, as it's not the root container in RML */
		left: 0px; top: 0px; right: 0px; bottom: 0px;
	}
	p {
		display: block;
		position: absolute;
		text-align: center;
	}
	p:hover {
		background: #444444;
	}
</style>
</head>
<body>
	<p data-model="modelMain"
	   data-style-left        = "labelRect.x + 'px'"
	   data-style-top         = "labelRect.y + 'px'"
	   data-style-width       = "labelRect.w + 'px'"
	   data-style-height      = "labelRect.h + 'px'"
	   data-style-line-height = "labelRect.h + 'px'">
		Hello world!
	</p>
</body>
</rml>
)";

class SceneMain : public Based::Scene {
	// Rml interface wrapper is managed by engine
	Based::RML::InterfaceHandle rml;
	// Rml context wrapper is managed by scene
	Based::RML::ContextStorage rctx {nullptr};
	// Rml models and documents wrappers are managed by context wrapper
	Based::RML::ModelHandle rmodel {nullptr};
	Based::RML::DocumentHandle rdoc {nullptr};

	Based::Rect2D<int> labelRect;
public:
	SceneMain (Based::Engine *engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine->client) return; // no client (headless)
		rml = engine->client->window()->rml.get();
		if (!rml) return;

		labelRect = conf["labelRect"];

		Based::RML::Interface::load_fonts ({"../demo/assets/common/courbd.ttf"});

		rctx = rml->make_context ("sceneMain");
		// Data Model must be defined before loading documents
		rmodel = rctx->add_model ("modelMain", Based::RML::BindTypes::Geometry);
		rmodel->constructor.Bind ("labelRect", &labelRect);

		rml->init_debugger (rctx, SDLK_F12);

		rdoc = rctx->add_document_from_string ("docMain", document_rml);
		rdoc->doc->Show();
	}

	// called only in client mode
	void handle_events (SDL_Event *event) override final {
		switch (event->type) {
			case SDL_EVENT_MOUSE_BUTTON_UP:
				if (!Rml::Debugger::IsVisible())
					engine->stop(); // same as engine->sceneManager.schedule_next (nullptr)
				else
					break;
			case SDL_EVENT_QUIT:
				Based::log.write ("Bye!");
				break;
			default: break;
		}
		if (rml) rml->handle_event (rctx, event);
	}

	// called in both client and headless modes
	void update () override final { }

	// called only in client mode
	void render () override final {
		if (rml) rml->render (rctx);
	}
};
