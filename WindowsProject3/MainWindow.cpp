
#include "MainWindow.h"

MainWindow::MainWindow()
{


	auto scene				= std::make_unique<Scene>();
	auto clear				= std::make_unique<Clear>();
	auto watchBackground	= std::make_unique<WatchBackground>();
	//auto text				= std::make_unique<SimpleText>(L"hvorfor kan jeg ikke sende over 20 mb jeg var med til at fucking compressed demdet ikke min skyld det er lidt sl�ret nude fucking idioter der har lave den fucking hjemmesidejeg ved godt hvorfor det tager s� lang tid til at f� handicap su fordi dem der skal l�se dem er fucking ogs� handicappetholt shit jeg har aldrig set s�dan et d�rlig systemplease sige jeg bliver f�rdig uddannet datamatiker s� jeg kan lave et meget bedre system end dem og s� kan jeg s�lge det for 500k de fucking aberjeg har aldrig v�rt s� sur over en fucking hjemmesidejeg var n�d til at compressed en til fil fordi DET ER MAKES 20MBBRO det dem der siger man skal sende s� meget som muligt s� det g�r hurtiger og s� de ikke skal kontakte hospitaler eller l�ger men s� har de et fucking cap p� hvor meget man m� sendealts� hvordan giver det meningnah det faktisk crazyog vente tiden er fucking 6 m�neder fordi de skal sidde og gok af mens de fucking nasser penge fra kommunen fordi de ikke laver deres fucking arbejede");

	scene->AddChild(std::move(clear));
	scene->AddChild(std::move(watchBackground));
	//scene->AddChild(std::move(text));

	auto graphicsModule = std::make_unique<GraphicsModule>(&hwnd, std::move(scene));
	modules.emplace_back(std::move(graphicsModule));
}