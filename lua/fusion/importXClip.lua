require "xclipioFusion"
ret = composition:AskUser("Select an XClip file (.xcl)", { {"XClip", "FileBrowse"} } )
importXClip(ret.XClip)

