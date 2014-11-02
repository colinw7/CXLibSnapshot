#include <CXLib.h>
#include <CFileUtil.h>
#include <COSTimer.h>
#include <cstdlib>

enum CSnapShotType {
  SNAPSHOT_ROOT,
  SNAPSHOT_WINDOW,
  SNAPSHOT_WINDOW_ID,
  SNAPSHOT_AREA,
  SNAPSHOT_BBOX
};

static CXScreen    *screen;
static CXWindow    *root;
static std::string  output_file;

static bool snapshot_root();
static bool snapshot_window();
static bool snapshot_window(Window id);
static bool snapshot_window(CXWindow *window);
static bool snapshot_area();
static bool snapshot_bbox(int x1, int y1, int x2, int y2);
static void usage();

int
main(int argc, char **argv)
{
  screen = CXMachineInst->getCXScreen(0);

  root = screen->getCXRoot();

  //output_file = "xsnapshot.xwd";
  output_file = "snapshot.png";

  CSnapShotType type          = SNAPSHOT_ROOT;
  Window        id            = 0;
  bool          animate       = false;
  int           animate_time  = 1000;
  int           animate_count = 10;

  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      char *opt = &argv[i][1];

      if      (strcmp(opt, "root"  ) == 0)
        type = SNAPSHOT_ROOT;
      else if (strcmp(opt, "window") == 0)
        type = SNAPSHOT_WINDOW;
      else if (strcmp(opt, "windowId") == 0) {
        if (i + 1 < argc)
          id = (Window) atoi(argv[++i]);
        else {
          fprintf(stderr, "Missing windowId argument\n");
          exit(1);
        }

        type = SNAPSHOT_WINDOW_ID;
      }
      else if (strcmp(opt, "area"  ) == 0)
        type = SNAPSHOT_AREA;
      else if (strcmp(opt, "bbox"  ) == 0) {
        type = SNAPSHOT_BBOX;

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
        fprintf(stderr, "Invalid option '-%s'\n", opt);
    }
    else
      output_file = argv[i];
  }

  if (animate) {
    CFile of(output_file);

    std::string output_root   = of.getDir() + "/" + of.getBase();
    std::string output_suffix = of.getSuffix();

    for (int i = 0; i < animate_count; ++i) {
      char nstr[32];

      sprintf(nstr, "%04d", i + 1);

      output_file = output_root + "_" + nstr + "." + output_suffix;

      bool rc = false;

      if      (type == SNAPSHOT_ROOT)
        rc = snapshot_root();
      else if (type == SNAPSHOT_WINDOW)
        rc = snapshot_window();
      else if (type == SNAPSHOT_WINDOW_ID)
        rc = snapshot_window(id);
      else if (type == SNAPSHOT_AREA)
        rc = snapshot_area();
      else if (type == SNAPSHOT_BBOX)
        rc = snapshot_bbox(x1, y1, x2, y2);

      if (! rc) continue;

      COSTimer::msleep(animate_time);
    }
  }
  else {
    bool rc = false;

    if      (type == SNAPSHOT_ROOT)
      rc = snapshot_root();
    else if (type == SNAPSHOT_WINDOW)
      rc = snapshot_window();
    else if (type == SNAPSHOT_WINDOW_ID)
      rc = snapshot_window(id);
    else if (type == SNAPSHOT_AREA)
      rc = snapshot_area();
    else if (type == SNAPSHOT_BBOX)
      rc = snapshot_bbox(x1, y1, x2, y2);

    if (! rc)
      exit(1);
  }

  return 0;
}

static bool
snapshot_root()
{
  CImagePtr image = root->getImage();

  CFileType type = CFileUtil::getImageTypeFromName(output_file);

  if (! image.isValid())
    return false;

  image->write(output_file, type);

  return true;
}

static bool
snapshot_window()
{
  CXWindow *window = CXMachineInst->selectWindow();

  if (window == NULL)
    return false;

  bool rc = snapshot_window(window);

  delete window;

  return rc;
}

static bool
snapshot_window(Window id)
{
  CXWindow *window = new CXWindow(id);

  if (window == NULL)
    return false;

  bool rc = snapshot_window(window);

  delete window;

  return rc;
}

static bool
snapshot_window(CXWindow *window)
{
  window->raiseWait(10);

  CImagePtr image = window->getImage();

  if (! image.isValid())
    return false;

  CFileType type = CFileUtil::getImageTypeFromName(output_file);

  image->write(output_file, type);

  return true;
}

static bool
snapshot_area()
{
  int start_x = 0, start_y = 0, end_x = 0, end_y = 0;

  CXMachineInst->selectRootRegion(&start_x, &start_y, &end_x, &end_y);

  return snapshot_bbox(start_x, start_y, end_x, end_y);
}

static bool
snapshot_bbox(int start_x, int start_y, int end_x, int end_y)
{
  if (start_x > end_x) std::swap(start_x, end_x);
  if (start_y > end_y) std::swap(start_y, end_y);

  if (start_x == end_x || start_y == end_y) {
    fprintf(stderr, "Invalid bbox (%d,%d) -> (%d,%d)\n", start_x, start_y, end_x, end_y);
    return false;
  }

  fprintf(stderr, "CXLibSnapshot -bbox %d %d %d %d\n", start_x, start_y, end_x, end_y);

  CImagePtr image = root->getImage(start_x, start_y, end_x - start_x, end_y - start_y);

  if (! image.isValid())
    return false;

  CFileType type = CFileUtil::getImageTypeFromName(output_file);

  image->write(output_file, type);

  return true;
}

static void
usage()
{
  fprintf(stderr, "Usage: CXLibSnapshot "
          "[-root|-window|-area|-bbox <x1> <y1> <x2> <y2>] [-help]\n");
  exit(0);
}
