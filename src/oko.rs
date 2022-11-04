use sysinfo::{System, SystemExt};

const NOT_RETRIEVED: &str = "NOT_RETRIEVED";

#[derive(Debug)]
pub struct Os {
    name: String,
    release: String,
}

impl Os {
    pub fn new() -> Self {
        Os {
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

pub fn collect_machine_info(os: &mut Os) {
    let mut sys = System::new_all();
    sys.refresh_all();

    os.name = sys.name().expect(NOT_RETRIEVED);
    os.release = sys.os_version().expect(NOT_RETRIEVED);
}
