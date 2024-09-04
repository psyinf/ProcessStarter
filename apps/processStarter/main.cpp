#include <processLib/core/processLib.hpp>
#include "processInfoLoader.hpp"
#include <processInfo.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>

#include "ftxui/component/captured_mouse.hpp"     // for ftxui
#include "ftxui/component/component.hpp"          // for Toggle, Renderer, Vertical
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"                 // for text, hbox, vbox, Element

using namespace ftxui;

Component MakeOne(Element&& e)
{
    class Impl : public ComponentBase
    {
    public:
        explicit Impl(Element&& e)
          : element_{std::move(e)}
        {
        }

        Element Render() final { return element_; }

    private:
        Element element_;
    };

    return Make<Impl>(std::move(e));
}

void renderTUI(const processStarter::ProcessInfoList& processInfos, std::vector<processLib::Process>& processes)
{
    auto container = Container::Vertical({});
    auto on_off = std::vector<std::string>{"Off", "On"};

    container->Add(MakeOne(text("Choose your options:")));

    for (const auto& [index, process] : processInfos.processInfoList | std::views::enumerate)
    {
        auto container_hbox = Container::Horizontal({});
        container_hbox->Add(MakeOne(text(process.name) | flex));
        // container_hbox->Add(Toggle(&on_off, &selections.at(index)));
        // button
        auto& process = processes.at(index);
        auto  isRunning = process.isRunning();
        container_hbox->Add(Button(isRunning ? "Stop" : "Start", [&] {
            auto isRunning2 = process.isRunning();
            if (isRunning2)
                process.stop();
            else
                process.start();
            isRunning2 = true;
        }));
        container->Add(container_hbox);
    }

    auto renderer = Renderer(container, [&] {
        // return container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 10) | border;
        return container->Render();
    });

    auto screen = ScreenInteractive::FitComponent();
    screen.SetCursor(ftxui::Screen::Cursor(0, 0, ftxui::Screen::Cursor::Hidden));
    screen.Loop(renderer);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    CLI::App    app{"Process Starter"};
    std::string processList = "../data/processList.yaml";
    app.add_option("-p, --processList", processList, "path to process list file");

    CLI11_PARSE(app, argc, argv);
    // load process list
    auto processInfo = processStarter::loadFromFile(processList);
    // make a list of processes
    std::vector<processLib::Process> processes;
    for (const auto& processInfo : processInfo.processInfoList)
    {
        processes.emplace_back(processLib::Process(
            processInfo.name, processInfo.path, processInfo.getArgumentString(), processInfo.policies));
    }
    // render TUI
    renderTUI(processInfo, processes);

    return 0;
}