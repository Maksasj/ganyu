import os
import hashlib
from urllib.request import urlretrieve
from datetime import datetime

def get_file_hash(path):
    return hashlib.md5(open(path,'rb').read()).hexdigest()

def traverse_physical_files(path):
    for root, dirs, files in os.walk(path):
        path = root.split(os.sep)
        # print((len(path) - 1) * '---', os.path.basename(root))
        for file in files:
            fileName = os.path.splitext(file)
            filePath = root + '\\' + file

            fileStats = os.stat(filePath) 

            modificationDateObject = datetime.fromtimestamp(os.path.getmtime(filePath))
            modificationDate = modificationDateObject.strftime('%Y-%m-%d')

            creationDateObject = datetime.fromtimestamp(os.path.getctime(filePath))
            creationDate = creationDateObject.strftime('%Y-%m-%d')

            hash = get_file_hash(filePath)

            formated = u"    (\'%s\', \'%s\', %d, \'%s\', DATE \'%s\', DATE \'%s', 3, \'%s\')," % (fileName[0],
                fileName[1],
                fileStats.st_size,
                hash,
                creationDate,
                modificationDate,
                root[3:])

            print(formated)

traverse_physical_files("C:\\Users\\maksa\\OneDrive\\Documents")
traverse_physical_files("C:\\Users\\maksa\\OneDrive\\Pictures")
# traverse_physical_files("C:\\Programming")

# webPaths = [
#     "https://cdn.donmai.us/sample/e8/f3/__original_drawn_by_takahashi_masaki__sample-e8f316fd575ecf0e4601988b6976aa56.jpg",
#     "https://cdn.donmai.us/original/36/27/__bolinoak_sammahn_and_palace_athene_gundam_and_1_more_drawn_by_ishibashi_ken_ichi__3627022d0f70a50c29337d51c3a00b49.jpg",
#     "https://cdn.donmai.us/original/69/91/__turn_a_gundam_loran_cehack_wadom_and_flat_gundam_and_1_more_drawn_by_syd_mead__69913a2c708a4e717e51b2f2796ded0c.jpg",
#     "https://cdn.donmai.us/original/ac/0c/__zeta_gundam_hyaku_shiki_and_super_gundam_gundam_and_1_more_drawn_by_ishibashi_ken_ichi__ac0c831d13d9c141f5058824e3df972d.jpg",
#     "https://cdn.donmai.us/original/4a/ee/__asshimar_gundam_and_1_more_drawn_by_hasegawa_masayuki__4aee9eeee0989a555a0120f9e791e64f.jpg",
#     "https://cdn.donmai.us/original/38/85/__marasai_gundam_and_1_more_drawn_by_hasegawa_masayuki__38856272adcb9f4c6bcc5714c67ff8e0.jpg",
#     "https://cdn.donmai.us/original/b6/26/__galbaldy_beta_gundam_and_1_more_drawn_by_ishibashi_ken_ichi__b62689d6cd58aba3b7a0b632a92c5c4a.jpg",
#     "https://cdn.donmai.us/original/77/a3/__yamato_uchuu_senkan_yamato_and_1_more_drawn_by_syd_mead__77a3488d646de591508e4d082af84639.jpg",
#     "https://cdn.donmai.us/sample/4a/a7/__gundam_mk_ii_gundam_and_1_more_drawn_by_syd_mead__sample-4aa717ed554b2c60734d6b443968e2b2.jpg",
#     "https://cdn.donmai.us/sample/03/a2/__nu_gundam_and_sazabi_gundam_and_1_more__sample-03a24be3a1e58f053738fa4e229cf62e.jpg",
#     "https://cdn.donmai.us/sample/b5/f8/__rx_78_2_gundam_and_1_more_drawn_by_ookawara_kunio__sample-b5f88b1806c2c105c51f4502e715913e.jpg",
#     "https://cdn.donmai.us/sample/9c/59/__rx_78_2_zaku_ii_s_char_custom_white_base_and_prototype_gundam_gundam_and_1_more_drawn_by_ookawara_kunio__sample-9c59b0a22f7c4d84026fd83336e150d9.jpg",
#     "https://cdn.donmai.us/original/62/8f/__rick_dias_gundam_and_1_more_drawn_by_ishibashi_ken_ichi__628f261eac3dcc09a8392345aa56ee9f.jpg",
#     "https://cdn.donmai.us/original/83/30/__rx_78_2_and_white_base_gundam_and_1_more_drawn_by_hasegawa_masayuki__8330727a4416ce7c909af3f947f20c5f.jpg",
#     "https://cdn.donmai.us/original/97/52/__ga_zowmn_gundam_and_1_more_drawn_by_hasegawa_masayuki__9752f37f77abbcad6199a3ffbd64cda4.jpg",
#     "https://cdn.donmai.us/original/58/94/__marasai_gundam_and_1_more_drawn_by_ishibashi_ken_ichi__58943f51631ea42936be03df2a03585c.jpg",
#     "https://cdn.donmai.us/original/83/39/__robin_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__83397a56917375819776f4c8219cf310.png",
#     "https://cdn.donmai.us/original/9d/de/__firefly_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__9dde0f9dee621aba02de951c0da366fa.png",
#     "https://cdn.donmai.us/original/7f/4c/__mika_blue_archive_drawn_by_kenomotsu_yukuwa__7f4c6478c6489bd681cc6fa22a85ae46.png",
#     "https://cdn.donmai.us/original/d9/b9/__trailblazer_stelle_and_firefly_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__d9b9a836618af299222fe682eb90b66f.png",
#     "https://cdn.donmai.us/original/a8/cc/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__a8ccaf7c16e7d51024917c2b19db0cdb.png",
#     "https://cdn.donmai.us/original/44/88/__sparkle_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__4488c943bc0589bf21ddd05779d5f7db.png",
#     "https://cdn.donmai.us/original/06/85/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__0685bd25b1352f50245e7137e1401941.png",
#     "https://cdn.donmai.us/original/37/4d/__arona_blue_archive_drawn_by_kenomotsu_yukuwa__374dd2c33960828f6ed8d455c7fcb317.png",
#     "https://cdn.donmai.us/original/c4/7a/__kamisato_ayaka_and_kamisato_ayaka_genshin_impact_drawn_by_kenomotsu_yukuwa__c47afe9b81b4e32f1f4c4440a35ee3e6.png",
#     "https://cdn.donmai.us/original/c6/b1/__frieren_sousou_no_frieren_drawn_by_kenomotsu_yukuwa__c6b1e6ea5bb7716e6e55cbd3fc61b68d.png",
#     "https://cdn.donmai.us/original/84/8b/__barbara_genshin_impact_drawn_by_kenomotsu_yukuwa__848bce1b0b76ccaff82b2fc2b34b5ef1.png",
#     "https://cdn.donmai.us/original/d9/8d/__fischl_takanashi_rikka_and_uchida_maaya_genshin_impact_and_1_more_drawn_by_kenomotsu_yukuwa__d98d5162a01e61c7c5ea512f3ef4d31c.png",
#     "https://cdn.donmai.us/original/87/a9/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__87a96c30ee373f32e940489d97562178.png",
#     "https://cdn.donmai.us/original/7b/db/__fu_xuan_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__7bdb7da39e0fbc21bd5c516b038ebb5c.png",
#     "https://cdn.donmai.us/original/a2/87/__hifumi_blue_archive_drawn_by_kenomotsu_yukuwa__a2879056fced9e80ed3ca88dd3303e51.png",
#     "https://cdn.donmai.us/original/ac/41/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__ac412ed94863b86222e1d0bd08fa8936.png",
#     "https://cdn.donmai.us/original/b2/97/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__b297390cb8e2cf7c46682a7caa8d84e6.png",
#     "https://cdn.donmai.us/original/88/73/__mutsuki_blue_archive_drawn_by_kenomotsu_yukuwa__88737fa847c1d78bf6cac1a5901f35c0.png",
#     "https://cdn.donmai.us/original/24/d5/__hatsune_miku_and_faruzan_genshin_impact_and_1_more_drawn_by_kenomotsu_yukuwa__24d53f835c526116ebc1314ff7715c57.png",
#     "https://cdn.donmai.us/original/d8/5a/__shiroko_and_shiroko_blue_archive_drawn_by_kenomotsu_yukuwa__d85a4185f0df5be45b1e207cdcf780b5.png",
#     "https://cdn.donmai.us/original/c2/69/__march_7th_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__c2690f8b993e530f6574e31afad69ee0.png",
#     "https://cdn.donmai.us/original/e1/84/__raiden_shogun_genshin_impact_and_1_more_drawn_by_kenomotsu_yukuwa__e184f887dd270a71f1444e53a3c11ae3.png",
#     "https://cdn.donmai.us/original/84/64/__silver_wolf_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__84649f2a119799d343da1eec403a38d3.png",
#     "https://cdn.donmai.us/original/98/15/__lumine_and_paimon_genshin_impact_drawn_by_kenomotsu_yukuwa__981575202b65b0065e1bb68b44038738.png",
#     "https://cdn.donmai.us/original/13/ad/__furina_genshin_impact_drawn_by_kenomotsu_yukuwa__13ad6b4bc5b2d7377b87ef02cc452fff.png",
#     "https://cdn.donmai.us/original/5d/cd/__myrtle_and_myrtle_arknights_drawn_by_kenomotsu_yukuwa__5dcd2c9497a94bc723c9fee05071b609.png",
#     "https://cdn.donmai.us/original/b8/d8/__march_7th_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__b8d83d6edb0df64243be5b3cd1a79c78.png",
#     "https://cdn.donmai.us/original/9d/8e/__qingque_honkai_and_1_more_drawn_by_kenomotsu_yukuwa__9d8e6a7ccc1a6bf781065a12b74d69d7.png",
#     "https://cdn.donmai.us/original/39/13/__shiroko_blue_archive_drawn_by_kenomotsu_yukuwa__391333a8733cd8e63882d939deaf72bb.png",
#     "https://cdn.donmai.us/original/bb/06/__hu_tao_megumin_boo_tao_and_takahashi_rie_genshin_impact_and_1_more_drawn_by_kenomotsu_yukuwa__bb06df12507cf480f34afb2641795369.png"
# ]

# def traverse_web_files(files):
#     for file in files:
#         url = file
# 
#         splitted = file.split('/')
# 
#         filename = url[39:] 
# 
#         urlretrieve(url, "web\\" + splitted[len(splitted) - 1])

# traverse_web_files(webPaths)
# traverse_physical_files("web")