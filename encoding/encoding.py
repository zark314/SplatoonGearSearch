# The purpose of this program is to solve
# the encoding issue across platforms

import os
SUFFIXES=[".c", ".h"]

# covert all source codes defined by SUFFIEXES in in_folder 
# from gbk encoding to utf8. Files after convertions are
# saved in out_folder
def codes_gbk_to_utf8(in_folder, out_folder):
    for file_name in os.listdir(in_folder):
        if not os.path.isfile(file_name): # ignore if folder
            continue
        if not any(file_name.endswith(suffix) for suffix in SUFFIXES):
            continue
        in_path = os.path.join(in_folder, file_name)
        out_path = os.path.join(out_folder, file_name)
        text = None
        with open(in_path, "r", encoding='gbk') as in_file:
            try:
                text = in_file.read()
            except:
                print("{} probably not gbk, skip".format(file_name))
                continue
            with open(out_path, "w", encoding="utf-8") as out_file:
                try:
                    out_file.write(text) 
                except:
                    print("failed to save {file_name} as utf-8")



if __name__ == "__main__":
    out_folder=os.path.dirname(__file__)
    in_folder = os.path.dirname(out_folder)
    codes_gbk_to_utf8(in_folder, out_folder)
