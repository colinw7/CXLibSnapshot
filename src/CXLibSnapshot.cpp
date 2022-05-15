#include <CXLib.h>
#include <CFileUtil.h>
#include <COSTimer.h>
#include <cstdlib>

enum class CSnapShotType {
  ROOT,
  WINDOW,
  WINDOW_ID,
  AREA,
  BBOX
};

class CSnapShot {
 public:
  CSnapShot();

  const std::string &outputFile() const { return outputFile_; }
  void setOutputFile(const std::string &s) { outputFile_ = s; }

  bool snapshotRoot();
  bool snapshotWindow();
  bool snapshotWindow(Window id);
  bool snapshotWindow(CXWindow *window);
  bool snapshotArea();
  bool snapshotBBox(int x1, int y1, int x2, int y2);

 private:
  CXScreen*   screen_ { nullptr };
  CXWindow*   root_   { nullptr };
  std::string outputFile_;
};

//------

void usage() {
  std::cerr << "Usage: CXLibSnapshot [-root|-window|-area|-bbox <x1> <y1> <x2> <y2>] [-help]\n";
  exit(0);
}

//------

int
main(int argc, char **argv)
{
  CSnapShot snapshot;

  std::string output_file = "snapshot.png";
  //std::string output_file = "xsnapshot.xwd";

  CSnapShotType type          = CSnapShotType::ROOT;
  Window        id            = 0;
  bool          animate       = false;
  int           animate_time  = 1000;
  int           animate_count = 10;

  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      char *opt = &argv[i][1];

      if      (strcmp(opt, "root"  ) == 0)
        type = CSnapShotType::ROOT;
      else if (strcmp(opt, "window") == 0)
        type = CSnapShotType::WINDOW;
      else if (strcmp(opt, "windowId") == 0) {
        if (i + 1 < argc)
          id = static_cast<Window>(atoi(argv[++i]));
        else {
          std::cerr << "Missing windowId argument\n";
          exit(1);
        }

        type = CSnapShotType::WINDOW_ID;
      }
      else if (strcmp(opt, "area"  ) == 0)
        type = CSnapShotType::AREA;
      else if (strcmp(opt, "bbox"  ) == 0) {
        type = CSnapShotType::BBOX;

        ++i; if (i < argc) x1 = atoi(argv[i]);
        ++i; if (i < argc) y1 = atoi(argv[i]);
        ++i; if (i < argc) x2 = atoi(argv[i]);
        ++i; if (i < argc) y2 = atoi(argv[i]);
      }
      else if (strcmp(opt, "animate") == 0) {
        animate = true;

        ++i; if (i < argc) animate_time  = atoi(argv[i]);
        ++i; if (i < argc) animate_count = atoi(argv[i]);
      }
      else if (strcmp(opt, "help"  ) == 0)
        usage();
      else
        std::cerr << "Invalid option '-" << opt << "'\n";
    }
    else {
      output_file = argv[i];
    }
  }

  if (animate) {
    CFile of(output_file);

    std::string output_root   = of.getDir() + "/" + of.getBase();
    std::string output_suffix = of.getSuffix();

    for (int i = 0; i < animate_count; ++i) {
      char nstr[32];

      sprintf(nstr, "%04d", i + 1);

      output_file = output_root + "_" + nstr + "." + output_suffix;

      snapshot.setOutputFile(output_file);

      bool rc = false;

      if      (type == CSnapShotType::ROOT)
        rc = snapshot.snapshotRoot();
      else if (type == CSnapShotType::WINDOW)
        rc = snapshot.snapshotWindow();
      else if (type == CSnapShotType::WINDOW_ID)
        rc = snapshot.snapshotWindow(id);
      else if (type == CSnapShotType::AREA)
        rc = snapshot.snapshotArea();
      else if (type == CSnapShotType::BBOX)
        rc = snapshot.snapshotBBox(x1, y1, x2, y2);

      if (! rc) continue;

      COSTimer::msleep(animate_time);
    }
  }
  else {
    snapshot.setOutputFile(output_file);

    bool rc = false;

    if      (type == CSnapShotType::ROOT)
      rc = snapshot.snapshotRoot();
    else if (type == CSnapShotType::WINDOW)
      rc = snapshot.snapshotWindow();
    else if (type == CSnapShotType::WINDOW_ID)
      rc = snapshot.snapshotWindow(id);
    else if (type == CSnapShotType::AREA)
      rc = snapshot.snapshotArea();
    else if (type == CSnapShotType::BBOX)
      rc = snapshot.snapshotBBox(x1, y1, x2, y2);

    if (! rc)
      exit(1);
  }

  return 0;
}

//------

CSnapShot::
CSnapShot()
{
  screen_ = CXMachineInst->getCXScreen(0);

  root_ = screen_->getCXRoot();
}

bool
CSnapShot::
snapshotRoot()
{
  CImagePtr image = root_->getImage();

  CFileType type = CFileUtil::getImageTypeFromName(outputFile());

  if (! image.isValid())
    return false;

  image->write(outputFile(), type);

  return true;
}

bool
CSnapShot::
snapshotWindow()
{
  CXWindow *window = CXMachineInst->selectWindow();

  if (! window)
    return false;

  bool rc = snapshotWindow(window);

  delete window;

  return rc;
}

bool
CSnapShot::
snapshotWindow(Window id)
{
  CXWindow *window = new CXWindow(id);

  if (! window)
    return false;

  bool rc = snapshotWindow(window);

  delete window;

  return rc;
}

bool
CSnapShot::
snapshotWindow(CXWindow *window)
{
  window->raiseWait(10);

  CImagePtr image = window->getImage();

  if (! image.isValid())
    return false;

  CFileType type = CFileUtil::getImageTypeFromName(outputFile());

  image->write(outputFile(), type);

  return true;
}

bool
CSnapShot::
snapshotArea()
{
  int start_x = 0, start_y = 0, end_x = 0, end_y = 0;

  CXMachineInst->selectRootRegion(&start_x, &start_y, &end_x, &end_y);

  return snapshotBBox(start_x, start_y, end_x, end_y);
}

bool
CSnapShot::
snapshotBBox(int start_x, int start_y, int end_x, int end_y)
{
  if (start_x > end_x) std::swap(start_x, end_x);
  if (start_y > end_y) std::swap(start_y, end_y);

  if (start_x == end_x || start_y == end_y) {
    std::cerr << "Invalid bbox (" << start_x << "," << start_y << ") -> (" <<
                                     end_x   << "," << end_y   << ")\n";
    return false;
  }

  std::cerr << "CXLibSnapshot -bbox " << start_x << " " << start_y << " " <<
                                         end_x   << " " << end_y   << "\n";

  CImagePtr image = root_->getImage(start_x, start_y, end_x - start_x, end_y - start_y);

  if (! image.isValid())
    return false;

  CFileType type = CFileUtil::getImageTypeFromName(outputFile());

  image->write(outputFile(), type);

  return true;
}
