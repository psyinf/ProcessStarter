#include <processLib/core/processLib.hpp>
#include "processInfoLoader.hpp"
#include <processInfo.hpp>
#include <iostream>
#include <fstream>

#include <string>
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

auto style = ButtonOption::Animated(Color::Default, Color::GrayDark, Color::Default, Color::White);

auto makeButtonContainer(const std::string& label, std::function<void()> action)
{
    auto buttonContainer = Container::Horizontal({});
    auto button = Button(label, action, style);
    buttonContainer->Add(button);
    return buttonContainer;
};

struct ProcessData
{
    processStarter::ProcessInfo          processInfo;
    std::shared_ptr<processLib::Process> process;
};

void renderTUI2(const std::vector<ProcessData>& processesData)
{
    std::string current = "";

    auto style = ButtonOption::Animated(Color::Default, Color::GrayDark, Color::Default, Color::White);

    auto container = Container::Vertical({});
    for (const auto& [index, processData] : (processesData | std::views::enumerate))
    {
        auto button = Button(
            "Button " + processData.processInfo.name, [&]() { processData.process->start(); }, style);
        container->Add(button);
    }
    auto renderProcess = [&](const ProcessData& info, Element e) {
        auto style = (info.process->isRunning()) ? dim : bold;

        return hbox({
            text(info.processInfo.name) | style | size(WIDTH, EQUAL, 20),
            separator(),
            text(info.process->isRunning() ? "Running" : "Stopped"),
            separator(),
            e,
        });
    };
    auto renderer = Renderer(container, [&] {
        std::vector<Element> entries;

        for (const auto& [index, process] : (processesData | std::views::enumerate))
            entries.push_back(renderProcess(process, container->ChildAt(index)->Render()));

        return vbox({
                   vbox(std::move(entries)),

               }) |
               border;
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
    auto processInfos = processStarter::loadFromFile(processList);
    // make a list of processes
    std::vector<std::shared_ptr<processLib::Process>> processes;
    for (const auto& processInfo : processInfos.processInfoList)
    {
        processes.emplace_back(std::make_shared<processLib::Process>(
            processInfo.name, processInfo.path, processInfo.getArgumentString(), processInfo.policies));
    }
    // render TUI
    // renderTUI(processInfo, processes);
    std::vector<ProcessData> processesData;
    for (size_t i = 0; i < processInfos.processInfoList.size(); ++i)
    {
        processesData.push_back({processInfos.processInfoList[i], processes[i]});
    }
    renderTUI2(processesData);
    return 0;
}