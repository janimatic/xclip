require "xclipioFusion"
ret = composition:AskUser("Select an XClip file (.xcl)", { {"XClip", "FileBrowse"} } )
exportXClip(ret.XClip)

