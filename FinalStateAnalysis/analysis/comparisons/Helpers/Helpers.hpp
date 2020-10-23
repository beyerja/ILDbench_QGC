namespace Helpers {

//------------------------------------------------------------------------------

void saveCanvas(shared_ptr<TCanvas> canvas, string output_base,
                vector<string> extensions = {"pdf", "jpg", "C", "root"}) {
  /** Save the given canvas for in all given extensions.
   **/
  for (const auto &extension : extensions) {
    canvas->Print((output_base + "." + extension).c_str());
  }
}

//------------------------------------------------------------------------------

} // namespace Helpers
