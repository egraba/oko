use sysinfo::{System, SystemExt};

const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

#[derive(Debug)]
pub struct OsInfo {
    name: String,
    release: String,
}

impl OsInfo {
    pub fn new() -> Self {
        OsInfo {
            name: String::new(),
            release: String::new(),
        }
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn release(&self) -> &str {
        &self.release
    }
}

pub fn collect_machine_info(os_info: &mut OsInfo) {
    let mut sys = System::new_all();
    sys.refresh_all();

    os_info.name = sys.name().expect(NOT_RETRIEVED);
    os_info.release = sys.os_version().expect(NOT_RETRIEVED);
}
