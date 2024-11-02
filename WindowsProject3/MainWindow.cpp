
#include "MainWindow.h"

MainWindow::MainWindow()
{


	auto scene				= std::make_unique<Scene>();
	auto clear				= std::make_unique<Clear>();
	auto watchBackground	= std::make_unique<WatchBackground>();
	//auto text				= std::make_unique<SimpleText>(L"hvorfor kan jeg ikke sende over 20 mb jeg var med til at fucking compressed demdet ikke min skyld det er lidt sløret nude fucking idioter der har lave den fucking hjemmesidejeg ved godt hvorfor det tager så lang tid til at få handicap su fordi dem der skal læse dem er fucking også handicappetholt shit jeg har aldrig set sådan et dårlig systemplease sige jeg bliver færdig uddannet datamatiker så jeg kan lave et meget bedre system end dem og så kan jeg sælge det for 500k de fucking aberjeg har aldrig vært så sur over en fucking hjemmesidejeg var nød til at compressed en til fil fordi DET ER MAKES 20MBBRO det dem der siger man skal sende så meget som muligt så det går hurtiger og så de ikke skal kontakte hospitaler eller læger men så har de et fucking cap på hvor meget man må sendealtså hvordan giver det meningnah det faktisk crazyog vente tiden er fucking 6 måneder fordi de skal sidde og gok af mens de fucking nasser penge fra kommunen fordi de ikke laver deres fucking arbejede");

	scene->AddChild(std::move(clear));
	scene->AddChild(std::move(watchBackground));
	//scene->AddChild(std::move(text));

	auto graphicsModule = std::make_unique<GraphicsModule>(&hwnd, std::move(scene));
	modules.emplace_back(std::move(graphicsModule));
}