idf_component_register(
    SRCS "Voice_Bot.c"
    INCLUDE_DIRS "."
    REQUIRES esp-sam
    PRIV_REQUIRES driver esp_timer
)
