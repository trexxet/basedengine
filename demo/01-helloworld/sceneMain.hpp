#pragma once

#include <memory>

#include <RmlUi/Debugger.h>

#include "Engine.hpp"
#include "Scene.hpp"

/* For this example, we'll embed the RML UI document.
 * Normally you'd want to load the document from a file. */
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
	/* Rml interface wrapper is managed by engine */
	Based::RML::InterfaceHandle rml;
	/* Rml context wrapper is managed by scene */
	Based::RML::ContextStorage rctx {nullptr};
	/* Rml models and documents wrappers are managed by context wrapper */
	Based::RML::ModelHandle rmodel {nullptr};
	Based::RML::DocumentHandle rdoc {nullptr};

	Based::Rect2D<int> labelRect;
public:
	SceneMain (Based::Engine& engine, Based::Lua::File& conf) : Based::Scene(engine) {
		if (!engine.client) return; // no client (headless)
		Based::WindowHandle window = engine.client->window();

		rml = window->rml.get();
		if (!rml) return;

		/* For this demo, we'll retrieve the label position from the Lua config
		 * and pass it to the RmlUI. */
		labelRect = conf["labelRect"];

		/* Fonts must be loaded before documents */
		Based::RML::Interface::load_fonts ({"../demo/assets/common/courbd.ttf"});

		/* Create RML context */
		rctx = rml->make_context ("sceneMain");

		/* Data Model must be defined before loading documents */
		rmodel = rctx->add_model ("modelMain", Based::RML::BindTypes::Geometry);
		if (!rmodel)
			Based::log.fatal ("Failed to create RML data model!");
		rmodel->constructor.Bind ("labelRect", &labelRect);

		/* RML debugger is useful for adjusting the layout of your UI */
		rml->init_rml_debugger (rctx, SDLK_F12);

		/* Debug overlay can show various information (FPS etc.) */
		window->debugOverlay.init ({"rmlui-debugger-font", Rml::Style::FontStyle::Normal,
		                           Rml::Style::FontWeight::Normal, 18, "white"}, SDLK_F3);

		/* Finally, add a document to the context and show it */
		rdoc = rctx->add_document_from_string ("docMain", document_rml);
		if (!rdoc)
			Based::log.fatal ("Failed to create RML document!");
		rdoc->doc->Show();
	}

	/* called only in client mode */
	void handle_event (SDL_Event *event) override final {
		switch (event->type) {
			case SDL_EVENT_MOUSE_BUTTON_UP:
				if (!Rml::Debugger::IsVisible())
					engine.stop(); /* same as engine.sceneManager.schedule_next (nullptr) */
				else
					break;
			case SDL_EVENT_QUIT:
				Based::log.write ("Bye!");
				break;
			default: break;
		}
		if (rml) rml->handle_event (rctx, event);
	}

	/* called in both client and headless modes */
	void update () override final { }

	/* called only in client mode */
	void render () override final {
		if (rml) rml->render (rctx);
	}
};
