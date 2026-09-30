#include <format>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

template <typename T, typename callback_t>
void process_event(const sf::Event& event, const callback_t& callback)
	{
	const T* instance_opt{event.getIf<T>()};
	if (instance_opt)
		{
		const T& instance{*instance_opt};
		callback(instance);
		}
	}

bool draw_vertices_array{false};
bool use_push_gl_state  {false};
bool draw_imgui         {false};

std::string window_title() noexcept
	{
	return std::format("draw_vertices_array [Q]: {}, use_push_gl_state [W]: {}, draw_imgui [E]: {}", draw_vertices_array, use_push_gl_state, draw_imgui);
	}

int main()
	{
	const sf::Texture texture{"test.png"};
	const sf::Sprite sprite  {texture   };

	const sf::VertexArray vertices_array{[]()
		{
		sf::VertexArray ret{sf::PrimitiveType::Lines, 2};
		ret[0].position = {  0.f,   0.f};
		ret[1].position = {100.f, 100.f};
		return ret;
		}()};

	sf::RenderWindow render_window{sf::VideoMode{{800, 600}}, window_title()};

	sf::Clock clock;

	if (!ImGui::SFML::Init(render_window)) { throw std::runtime_error{"ImGui SFML error, could not update font texture."}; }


	while (render_window.isOpen())
		{
		if (const auto event_opt{render_window.waitEvent()})
			{
			const auto& event{*event_opt};
			process_event<sf::Event::Closed    >(event, [&](const auto& event) { render_window.close(); });
			process_event<sf::Event::KeyPressed>(event, [&](const auto& event)
				{
				if (event.code == sf::Keyboard::Key::Q) { draw_vertices_array = !draw_vertices_array; }
				if (event.code == sf::Keyboard::Key::W) { use_push_gl_state   = !use_push_gl_state  ; }
				if (event.code == sf::Keyboard::Key::E) { draw_imgui          = !draw_imgui         ; }
				render_window.setTitle(window_title());
				});

			ImGui::SFML::ProcessEvent(render_window, event);
			}

		//Step
			{
			if (draw_imgui)
				{
				ImGui::SFML::Update(render_window, clock.restart());
				}

			//ImGui::Begin("##Main", 0, ImGuiWindowFlags_None);
			//ImGui::End();
			}

		//Draw
			{
			render_window.clear();

			if (use_push_gl_state) 
				{
				render_window.pushGLStates(); 
				}
			
			render_window.draw(sprite);

			if (draw_vertices_array)
				{
				render_window.draw(vertices_array);
				}

			if (use_push_gl_state)
				{
				render_window.popGLStates();
				}

			if (draw_imgui)
				{
				ImGui::SFML::Render(render_window);
				}
			render_window.display();
			}
		}
	}