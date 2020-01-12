package de.pixinger77.openregattastartsystem;

import java.util.HashMap;
import java.util.Map;

public enum RaceFlags {
    Alphabet_Papa(0),
    Alphabet_Uniform(1),
    Alphabet_Xray(2),
    Answer_Staretverschiebung(3),
    Substander_1(4);

    private int value;
    private static Map map = new HashMap<>();

    private RaceFlags(int value) {
        this.value = value;
    }

    static {
        for (RaceFlags pageType : RaceFlags.values()) {
            map.put(pageType.value, pageType);
        }
    }

    public static RaceFlags valueOf(int pageType) {
        return (RaceFlags) map.get(pageType);
    }

    public int getValue() {
        return value;
    }
}
