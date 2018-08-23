import os

def find_file_from_parent(current_path, file_name):
    file_path = os.path.abspath(os.path.join(current_path, file_name))
    has_file = os.path.isfile(file_path)
    if has_file:
        return file_path
    while not has_file:
        current_path = os.path.abspath(os.path.join(current_path, os.path.pardir))
        if not os.path.exists(current_path):
            return None
        file_path = os.path.join(current_path, file_name)
        has_file = os.path.isfile(file_path)
        if has_file:
            return file_path
