#include <iostream>

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

void apply_zoom(float zoom, sf::RenderTarget& render_target) noexcept
	{
	const auto viewport_size{static_cast<sf::Vector2f>(render_target.getSize())};
	auto view{render_target.getView()};
	view.setSize(viewport_size * zoom);
	render_target.setView(view);
	}

sf::VertexArray create_pixels_grid(const sf::Texture& texture) noexcept
	{
	//const auto texture_size{texture.getSize()};
	//const sf::Vector2u lines{texture_size.x + 1, texture_size.y + 1};
	//sf::VertexArray vertices_array{sf::PrimitiveType::Lines, (lines.x * 2) + (lines.y * 2)};
	//std::cout << "Vertices count: " << vertices_array.getVertexCount() << "\n";
	//
	//for (unsigned int x{0}; x < lines.x; x++)
	//	{
	//	vertices_array[(x * 2) + 0].position = {static_cast<float>(x), static_cast<float>(0)};
	//	vertices_array[(x * 2) + 1].position = {static_cast<float>(x), static_cast<float>(lines.y)};
	//	}
	//const auto y_base_index{lines.x * 2};
	//for (unsigned int y{0}; y < lines.y; y++)
	//	{
	//	vertices_array[y_base_index + (y * 2) + 0].position = {static_cast<float>(0), static_cast<float>(y)};
	//	vertices_array[y_base_index + (y * 2) + 1].position = {static_cast<float>(lines.x), static_cast<float>(y)};
	//	}
	//for (auto& vertex : vertices_array)
	//	{
	//	vertex.color = {230, 240, 255, 80};
	//	}
	//return vertices_array;
	sf::VertexArray vertices_array{sf::PrimitiveType::Lines, 2};
	vertices_array[0].position = {0.f, 0.f};
	vertices_array[1].position = {100.f, 100.f};
	return vertices_array;
	}

int main()
	{
	//Prepare image to display
	sf::Texture texture{"test.png"};
	sf::Sprite sprite{texture};
	sf::VertexArray pixels_grid{create_pixels_grid(texture)};

	sf::RenderWindow render_window{sf::VideoMode{{800, 600}}, "Pixel grid"};

	auto view{render_window.getView()};
	view.setCenter(static_cast<sf::Vector2f>(texture.getSize() / 2u));
	render_window.setView(view);

	sf::Clock clock;

	if (!ImGui::SFML::Init(render_window)) { throw std::runtime_error{"ImGui SFML error, could not update font texture."}; }

	float zoom{1.f};
	while (render_window.isOpen())
		{
		if (const auto event_opt{render_window.waitEvent()})
			{
			const auto& event{*event_opt};
			process_event<sf::Event::Closed >(event, [&](const auto& event) { render_window.close(); });
			process_event<sf::Event::Resized>(event, [&](const auto& event) { apply_zoom(zoom, render_window); });
			process_event<sf::Event::MouseWheelScrolled>(event, [&](const auto& event)
				{
				zoom += zoom * .1f * -event.delta;
				zoom = std::clamp(zoom, 0.1f, 2.f);
				apply_zoom(zoom, render_window);
				});

			ImGui::SFML::ProcessEvent(render_window, event);
			}

		if (true)//Step
			{
			ImGui::SFML::Update(render_window, clock.restart());

			ImGui::Begin("##Main", 0, ImGuiWindowFlags_None);
			ImGui::Text("Text");
			ImGui::End();
			}

		if (true)//Draw
			{
			render_window.clear();
			render_window.draw(sprite);
			if (zoom <= .2f)
				{
				//render_window.draw(pixels_grid);
				}

			ImGui::SFML::Render(render_window);
			render_window.display();
			}
		}
	}