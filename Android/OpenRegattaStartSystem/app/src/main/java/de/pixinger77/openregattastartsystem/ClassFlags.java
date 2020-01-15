package de.pixinger77.openregattastartsystem;

import java.util.HashMap;
import java.util.Map;

public enum ClassFlags {
    Unknown(-1),
    Open(0),
    Opti(1),
    Class420(2),
    Europe(3),
    Ixylon(4),
    Laser(5),
    RS(6);

    private int value;
    private static Map map = new HashMap<>();

    private ClassFlags(int value) {
        this.value = value;
    }

    static {
        for (ClassFlags pageType : ClassFlags.values()) {
            map.put(pageType.value, pageType);
        }
    }

    public static ClassFlags valueOf(int pageType) {
        return (ClassFlags) map.get(pageType);
    }

    public int getValue() {
        return value;
    }
    public int getRessourceId() {
        switch (value) {
            case 0:
                return R.mipmap.ic_flag_class_open;
            case 1:
                return R.mipmap.ic_flag_class_opti;
            case 2:
                return R.mipmap.ic_flag_class_420er;
            case 3:
                return R.mipmap.ic_flag_class_europe;
            case 4:
                return R.mipmap.ic_flag_class_ixylon;
            case 5:
                return R.mipmap.ic_flag_class_laser;
            case 6:
                return R.mipmap.ic_flag_class_rs;
            default:
                return R.drawable.ic_flag_class_unknown;
        }
    }
}
