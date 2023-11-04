





struct header {
    uint16_t v1;
    uint16_t v2;
    uint32_t v3;
};
struct data1 {
    uint32_t d1;
};

void htonHead(struct header h, char buffer[6])
{
	uint16_t u16;
	uint32_t u32;
    u16 = htons(h.v1);
    memcpy(buffer, &u16, 2);
    u16 = htons(h.v2);
    memcpy(buffer+2, &u16, 2);
    u16 = htons(h.v3);
    memcpy(buffer+4, &u16, 2);
}
//void htonData(struct data1 d, char buffer[4])
//{
//    uint32_t u32;
//    u32 = htons(d.d1);
//    memcpy(buffer + 4, &u32, 4);
//}

 void htonAll(struct header h, struct data1 d, char buffer[12])
{
     htonHead(h, buffer);
    // htonData(d, buffer);
}